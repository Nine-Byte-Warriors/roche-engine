#include "stdafx.h"
#include "Level.h"
#include "ProjectileEditor.h"

#if _DEBUG
extern bool g_bDebug;
#include "MouseCapture.h"
#endif

void Level::OnCreate()
{
	try
	{
		// Initialize constant buffers
		HRESULT hr = m_cbMatrices.Initialize( m_gfx->GetDevice(), m_gfx->GetContext() );
		COM_ERROR_IF_FAILED( hr, "Failed to create 'Matrices' constant buffer!" );

        // Initialize 2d camera
        XMFLOAT2 aspectRatio = { static_cast<float>( m_gfx->GetWidth() ), static_cast<float>( m_gfx->GetHeight() ) };
        m_camera.SetProjectionValues( aspectRatio.x, aspectRatio.y, 0.0f, 1.0f );

        // Initialize systems
        m_textRenderer.Initialize( m_gfx->GetDevice(), m_gfx->GetContext() );

		// Initialise Projectile Editor
        m_projectileEditor = ProjectileEditor::CreateEditor();
		m_projectileEditor->Initialise(*m_gfx, m_cbMatrices);
	}
	catch ( COMException& exception )
	{
		ErrorLogger::Log( exception );
        return;
	}
}

void Level::CreateEntity()
{
#ifdef _DEBUG
#else
    m_entityController.RemoveEnemiesFromEntityData();
#endif

    m_fMaxHealth = 0.01;
    *m_fCurrentHealth = 0.01;
    EventSystem::Instance()->AddEvent(EVENTID::EnemyMaxHealth, &m_fMaxHealth);
    EventSystem::Instance()->AddEvent(EVENTID::EnemyCurrentHealth, m_fCurrentHealth);

    m_entity.clear();
    m_iEntityAmount = m_entityController.GetSize();
    for (int i = 0; i < m_iEntityAmount; i++)
    {
        Entity *entityPop = new Entity(m_entityController, i);
        m_entity.push_back(*entityPop);
        m_entity[i].SetCollisionHandler(&m_collisionHandler);
        m_entity[i].Initialize(*m_gfx, m_cbMatrices);
        m_entity[i].SetProjectileManagerInit(*m_gfx, m_cbMatrices);

        DisplayEntityMaxHealth(i);

        if (m_entityController.HasCollider(i))
        {
            m_collisionHandler.AddCollider(m_entity[i].GetCollider());
        }

        delete entityPop;
    }

    for (int i = 0; i < m_entity.size(); i++)
    {
        if (m_entity[i].GetType() == "Player")
        {
            m_entity[i].GetInventory()->LoadInventory();
        }
    }
}

void Level::CreateUI()
{
    // Update user interface
    EventSystem::Instance()->AddEvent( EVENTID::ShowCursorEvent );
    m_ui->RemoveAllUI();
    for ( unsigned int i = 0; i < m_uiEditor.GetScreens().size(); i++ )
	    m_ui->AddUI( m_uiEditor.GetScreens()[i], m_uiEditor.GetScreenData()[i].name );

    // find the player in the entity vector
    int playerIdx = 0;
    for ( unsigned int i = 0; i < m_entity.size(); i++ )
    {
        if ( m_entity[i].GetType() == "Player" )
        {
            playerIdx = i;
            break;
        }
    }

    m_ui->Initialize( *m_gfx, &m_cbMatrices, m_uiEditor.GetWidgets(), *m_entity[playerIdx].GetHealth() );
    m_ui->HideAllUI();
    EventSystem::Instance()->AddEvent(EVENTID::LevelOnCreateUI);
}

void Level::CreateTileMap()
{
    m_iTileMapRows =  2 * m_gfx->GetHeight() / m_iTileSize;
    m_iTileMapColumns = 2 * m_gfx->GetWidth() / m_iTileSize;
    m_tileMapPaintOnMap.Initialize(m_camera, m_iTileMapRows, m_iTileMapColumns, m_gfx->GetHeight() / 2, m_gfx->GetWidth() / 2);
    m_tileMapLoader.Initialize(m_iTileMapRows, m_iTileMapColumns);

#ifdef _DEBUG
    m_tileMapEditor.Initialize(m_iTileMapRows, m_iTileMapColumns);
    m_tileMapLoader.SetLevel(m_tileMapEditor.GetLevel(TileMapLayer::Background), m_tileMapEditor.GetLevel(TileMapLayer::Foreground));
#else
    m_tileMapLoader.LoadLevel("67x120file.json", "67x120file.json");
#endif

    CreateTileMapDraw();
}

void Level::CreateTileMapDraw()
{
    const int startingPosX = 0 - m_gfx->GetWidth() / 2;
    const int startingPosY = 0 - m_gfx->GetHeight() / 2;
    const int gapBetweenTiles = 0;
    int colPositionTotalTileLength = startingPosX;
    int rowPositionTotalTileLength = startingPosY;

    for (int i = 0; i < m_iTileMapLayers; i++)
    {
        std::vector<TileMapDraw>* tileMapDraw = new std::vector<TileMapDraw>;

        for (int j = 0; j < m_iTileMapRows * m_iTileMapColumns; j++)
        {
            TileMapDraw tileMapDrawPop;
            tileMapDraw->push_back(tileMapDrawPop);
            (*tileMapDraw)[j].Initialize(*m_gfx, m_cbMatrices, "NONE");

            if (j != 0)
            {
                colPositionTotalTileLength += m_iTileSize + gapBetweenTiles;
            }
            bool endOfRow = j % m_iTileMapColumns == 0 && j != 0;
            if (endOfRow)
            {
                rowPositionTotalTileLength += m_iTileSize + gapBetweenTiles;
                colPositionTotalTileLength = startingPosX;
            }

            float positionWidth = colPositionTotalTileLength;
            float positionHeight = rowPositionTotalTileLength;

            (*tileMapDraw)[j].GetTransform()->SetPositionInit(positionWidth, positionHeight);
            (*tileMapDraw)[j].GetSprite()->SetWidthHeight( (float)m_iTileSize, (float)m_iTileSize );
        }

        m_tileMapDrawLayers.push_back(*tileMapDraw);
        tileMapDraw->clear();
        delete tileMapDraw;

        colPositionTotalTileLength = startingPosX;
        rowPositionTotalTileLength = startingPosY;
    }
}

void Level::OnSwitch()
{
	// Update level system
    EventSystem::Instance()->AddEvent( EVENTID::ShowCursorEvent );
    CreateEntity();
    CreateUI();
}

void Level::BeginFrame()
{
	// Setup pipeline state
	m_gfx->BeginFrame();
	m_gfx->UpdateRenderState();
}

void Level::RenderFrame()
{
	auto gfxContext = m_gfx->GetContext();
	auto camMatrix = m_camera.GetWorldOrthoMatrix();

    // Sprites
    RenderFrameTileMap();

    m_projectileEditor->Draw(gfxContext, camMatrix);

    RenderFrameEntity();
}

void Level::RenderFrameEntity()
{
    for (int i = 0; i < m_entity.size(); i++)
    {
        if (m_entity[i].GetSprite())
        {
            m_entity[i].GetSprite()->UpdateBuffers(m_gfx->GetContext());
            m_entity[i].GetSprite()->Draw(m_entity[i].GetTransform()->GetWorldMatrix(), m_camera.GetWorldOrthoMatrix());

            if (m_entityController.HasProjectileBullet(i))
            {
                for (std::shared_ptr<ProjectileManager>& pManager : m_entity[i].GetProjectileManagers())
                    pManager->Draw(m_gfx->GetContext(), m_camera.GetWorldOrthoMatrix());
            }
        }
    }
}

void Level::RenderFrameTileMap()
{
    int layerSize = m_tileMapDrawLayers.size();
#if _DEBUG
    if (m_tileMapEditor.GetTileMapLayer() == TileMapLayer::Foreground)
    {
        layerSize = 2;
    }
    else if (m_tileMapEditor.GetTileMapLayer() == TileMapLayer::Background)
    {
        layerSize = 1;
    }
#endif

    for (int i = 0; i < layerSize; i++)
    {
        for (int k = 0; k <= (m_gfx->GetHeight() / m_iTileSize) + 1; k++)
        {
            int height = (k - 1) * ((m_gfx->GetHeight() / m_iTileSize) - 1);
            int topLeftPos = m_tileMapPaintOnMap.GetPositionAtCoordinates(0, height);
            int topRightPos = m_tileMapPaintOnMap.GetPositionAtCoordinates(m_gfx->GetWidth() + m_iTileSize, height);

            for (unsigned j = topLeftPos; j < topRightPos; j++)
            {
                m_tileMapDrawLayers[i][j].GetSprite()->UpdateBuffers(m_gfx->GetContext());
                m_tileMapDrawLayers[i][j].GetSprite()->Draw(m_tileMapDrawLayers[i][j].GetTransform()->GetWorldMatrix(), m_camera.GetWorldOrthoMatrix());
            }
        }
    }
}

void Level::SpawnFinalBoss()
{
    int entitynum = m_entityController.GetEntityEnemyNumFromName("Corn");
    std::string texture = m_entityController.GetTexture(entitynum);

    Vector2f spawnpos = Vector2f(500, 500);
    m_entitySpawner.AddEntityToSpawn(entitynum, 0, spawnpos);

    m_entitySpawner.SpawnEntity(0);

    m_entityController.AddEntityData(m_entitySpawner.GetEntityData()[0]);

    m_entitySpawner.EntitiesAdded();
}

void Level::EndFrame_Start()
{
    // Render ui
    m_ui->Draw(
        m_gfx->GetShaderVtx(), m_gfx->GetShaderPix(),
        m_camera.GetOrthoMatrix(), &m_textRenderer );

    // Render scene to texture
    m_gfx->RenderSceneToTexture();

#if _DEBUG
    if ( g_bDebug )
    {
        // Render imgui windows
        m_imgui->BeginRender();
        m_imgui->SpawnMenuBar();

        ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 0.0f, 0.0f ) );
        if ( ImGui::Begin( "Scene Window", FALSE ) )
        {

            ImVec2 vRegionMax = ImGui::GetWindowContentRegionMax();
            ImVec2 vImageMax = ImVec2(
                vRegionMax.x + ImGui::GetWindowPos().x,
                vRegionMax.y + ImGui::GetWindowPos().y );

            ImVec2 vRatio =
            {
                m_gfx->GetWidth() / ImGui::GetWindowSize().x,
                m_gfx->GetHeight() / ImGui::GetWindowSize().y
            };

            bool bIsFitToWidth = vRatio.x < vRatio.y ? true : false;
            ImVec2 ivMax =
            {
                bIsFitToWidth ? m_gfx->GetWidth() / vRatio.y : vRegionMax.x,
                bIsFitToWidth ? vRegionMax.y : m_gfx->GetHeight() / vRatio.x
            };

            ImVec2 pos = ImGui::GetCursorScreenPos();
            Vector2f half = { ( ivMax.x - vRegionMax.x ) / 2, ( ivMax.y - vRegionMax.y ) / 2 };
            ImVec2 vHalfPos = { pos.x - half.x, pos.y - half.y };

            ImVec2 ivMaxPos =
            {
                ivMax.x + ImGui::GetWindowPos().x - half.x,
			    ivMax.y + ImGui::GetWindowPos().y - half.y
            };

            ImGui::GetWindowDrawList()->AddImage(
                (void*)m_gfx->GetRenderTargetPP()->GetShaderResourceView(),
                vHalfPos, ivMaxPos );

            if ( ImGui::IsWindowHovered() )
            {
                ImGuiIO& io = ImGui::GetIO();
                Vector2f windowPos = Vector2f( ImGui::GetWindowPos().x, ImGui::GetWindowPos().y );
                ImVec2 gameSize = ImVec2( m_gfx->GetWidth(), m_gfx->GetHeight() );
			    Vector2f* vFakedPos = new Vector2f(
                    MouseCapture::GetGamePos( io.MousePos,
                        Vector2f( vHalfPos.x, vHalfPos.y ),
                        { ivMax.x, ivMax.y - 20.0f }, gameSize ) );
                EventSystem::Instance()->AddEvent( EVENTID::ImGuiMousePosition, vFakedPos );

                ImGui::Text("On Screen");
                ImGui::SameLine();
                std::string sFakedMouseText =
                    " X: " + std::to_string(vFakedPos->x) +
                    " Y: " + std::to_string(vFakedPos->y);
                ImGui::Text(sFakedMouseText.c_str());
                m_bIsWindowHovered = true;
            }
            else
            {
                m_bIsWindowHovered = false;
            }
        }
        ImGui::End();
        ImGui::PopStyleVar();

        m_gfx->SpawnControlWindow();
        m_uiEditor.SpawnControlWindow( *m_gfx );
        m_projectileEditor->SpawnEditorWindow(*m_gfx, m_cbMatrices);
        m_entityEditor.SpawnControlWindow(m_gfx->GetWidth(), m_gfx->GetHeight());
        m_tileMapEditor.SpawnControlWindow();
        m_audioEditor.SpawnControlWindow();
        m_camera.SpawnControlWindow();
    }
#endif
}

void Level::EndFrame_End()
{
#if _DEBUG
    if ( g_bDebug )
        m_imgui->EndRender();
#endif

    // Present Frame
	m_gfx->EndFrame();
}

void Level::Update( const float dt )
{
    if (m_bIsGamePaused == false) {
        UpdateTileMap(dt);
        UpdateEntity(dt);

        m_projectileEditor->Update(dt);
        m_collisionHandler.Update();
        m_camera.Update(dt);
    }

    UpdateUI(dt);
}

void Level::UpdateUI( const float dt )
{
#if _DEBUG
    if ( m_uiEditor.GetShouldUpdate() )
    {
        CreateUI();
        m_uiEditor.SetShouldUpdate( false );
    }
#endif
    m_uiEditor.Update( dt );
    if ( m_uiEditor.GetCurrentScreenIndex() > -1 )
    {
        m_ui->HideAllUI();
        m_ui->ShowUI( m_uiEditor.GetCurrentScreenName() );
    }
#if _DEBUG
    if ( m_uiEditor.ShouldShowAll() )
        m_ui->ShowAllUI();
    if ( m_uiEditor.ShouldHideAll() )
        m_ui->HideAllUI();
#endif
    m_ui->Update( dt );
}

void Level::UpdateEntity(const float dt)
{
#if _DEBUG
    m_entityController.SetEntityData(m_entityEditor.GetEntityData());
#endif

    if (m_iEntityAmount < m_entityController.GetSize())
    {
        AddNewEntity();
    }
#if _DEBUG
    else if (m_iEntityAmount != m_entityController.GetSize() || m_entityController.HasComponentUpdated() || m_entityController.GetDead().size() != 0)
#endif
    {
        RemoveEntities();
    }
    
#if _DEBUG
    for (int i = 0; i < m_iEntityAmount; i++)
    {
        m_entity[i].UpdateFromEntityData(dt, m_entityEditor.IsPositionLocked());
    }
#endif

    for (int i = 0; i < m_entity.size(); i++)
    {
        if (m_entity[i].GetSprite())
        {
            m_entity[i].Update(dt);
            DisplayEntityCurrentHealth(i);
        }
    }
}

void Level::AddNewEntity()
{
    for (int i = m_iEntityAmount; i < m_entityController.GetSize(); i++)
    {
        Entity* entityPop = new Entity(m_entityController, i);
        m_entity.push_back(*entityPop);
        m_entity[i].SetCollisionHandler(&m_collisionHandler);
        m_entity[i].Initialize(*m_gfx, m_cbMatrices);
        m_entity[i].SetProjectileManagerInit(*m_gfx, m_cbMatrices);
        delete entityPop;

        DisplayEntityMaxHealth(i);

        if (m_entityController.HasCollider(i))
        {
            m_collisionHandler.AddCollider(m_entity[i].GetCollider());
        }

        m_entity[i].GetSprite()->UpdateBuffers(m_gfx->GetContext());
        m_entity[i].GetSprite()->Draw(m_entity[i].GetTransform()->GetWorldMatrix(), m_camera.GetWorldOrthoMatrix());

        if (m_entityController.HasProjectileSystem(i))
        {
            for (std::shared_ptr<ProjectileManager>& pManager : m_entity[i].GetProjectileManagers())
				pManager->Draw(m_gfx->GetContext(), m_camera.GetWorldOrthoMatrix());
        }

    }

    m_iEntityAmount = m_entityController.GetSize();
    m_entityController.UpdateCopy();
}

void Level::RemoveEntities()
{    
#if _DEBUG
    m_entitiesDeleted = m_entityEditor.GetEntitiesDeleted();
#else
    for (int i = 0; i < m_entity.size(); i++)
    {
        if (m_entity[i].GetHealth() && m_entity[i].GetHealth()->GetCurrentHealth() <= 0)
        {
            m_collisionHandler.RemoveCollider(m_entity[i].GetCollider());
            std::string texture = "Resources\\Textures\\Tiles\\transparent.png";
            m_entity[i].GetSprite()->UpdateTex(m_gfx->GetDevice(), texture);
            m_entity[i].GetAI()->SetBehaviour(AILogic::AIStateTypes::Idle);
            m_entity[i].GetTransform()->SetPosition(-9999, -9999);
        }
    }
#endif

#if _DEBUG
    for (int i = 0; i < m_entitiesDeleted.size(); i++)
    {
        m_collisionHandler.RemoveCollider(m_entity[m_entitiesDeleted[i]].GetCollider());
        m_entity.erase(m_entity.begin() + m_entitiesDeleted[i]);
    }

    m_entitiesDeleted.clear();

    for (int i = 0; i < m_entity.size(); i++)
        m_entity[i].UpdateEntityNum(i);

    m_iEntityAmount = m_entityEditor.GetEntityData().size();
    m_entityEditor.ClearEntitiesDeleted();
#else
    m_iEntityAmount = m_entityController.GetSize();
#endif
    m_entityController.ClearDead();
}

void Level::CleanUpEntities()
{
#if _DEBUG
#else
    for (int i = 0; i < m_entity.size(); i++)
    {
        if (m_entity[i].GetType() != "Player" && m_entity[i].GetType() != "Decoration")
        {
            m_collisionHandler.RemoveCollider(m_entity[i].GetCollider());
            std::string texture = "Resources\\Textures\\Tiles\\transparent.png";
            if (m_entity[i].GetSprite())
            {
                m_entity[i].GetSprite()->UpdateTex(m_gfx->GetDevice(), texture);
            }
            if (m_entity[i].GetAI())
            {
                m_entity[i].GetAI()->SetBehaviour(AILogic::AIStateTypes::Idle);
            }
            if (m_entity[i].GetTransform())
            {
                m_entity[i].GetTransform()->SetPosition(-9999, -9999);
            }
        }
    }
#endif
}

void Level::DisplayEntityMaxHealth(int num)
{
    if (m_entity[num].GetType() == "Enemy")
    {
        float* maxHealthPtr = new float;
        m_fMaxHealth += m_entity[num].GetHealth()->GetMaxHealth();
        *maxHealthPtr = m_fMaxHealth;
        EventSystem::Instance()->AddEvent(EVENTID::EnemyMaxHealth, maxHealthPtr);
    }
}

void Level::DisplayEntityCurrentHealth(int num)
{
    if (num == 0)
    {
        *m_fCurrentHealth = 0;
    }
    if (m_entity[num].GetType() == "Enemy")
    {
        *m_fCurrentHealth += m_entity[num].GetHealth()->GetCurrentHealth();
    }
    if (num == m_iEntityAmount - 1)
    {
        EventSystem::Instance()->AddEvent(EVENTID::EnemyCurrentHealth, m_fCurrentHealth);
    }
}

void Level::UpdateTileMap(const float dt)
{
#if _DEBUG
    if (m_tileMapEditor.GetTileMapLayer() == TileMapLayer::Background)
    {
        UpdateTileMapTexture(dt);
        UpdateTileMapEmpty(dt);
    }
    else
    {
        UpdateBothTileMaps(dt);
    }

    bool isDrawOnMapAvalible = m_tileMapPaintOnMap.IsLeftMouseDown() && m_bIsWindowHovered && m_tileMapEditor.IsDrawContinuousAvalible();
    if (isDrawOnMapAvalible)
    {
        std::string texture = m_tileMapEditor.GetTexture();
        int pos = m_tileMapPaintOnMap.GetTileMapPos();

        m_tileMapEditor.UpdateTileMap(pos);

        if (m_tileMapEditor.GetTileMapLayer() == TileMapLayer::Background)
        {
            m_tileMapDrawLayers[0][pos].GetSprite()->UpdateTex(m_gfx->GetDevice(), texture);
        }
        else if (m_tileMapEditor.GetTileMapLayer() == TileMapLayer::Foreground)
        {
            m_tileMapDrawLayers[1][pos].GetSprite()->UpdateTex(m_gfx->GetDevice(), texture);
        }
    }

#else
    if (m_bMapUpdate)
    {
        UpdateBothTileMaps(dt);
        m_bMapUpdate = false;
    }
#endif

    UpdateTileMapPlanting(dt);
}

void Level::UpdateTileMapPlanting(const float dt)
{
    int player = m_entityController.GetEntityNumFromName("Player");
    int drawLayer = 1;
    float radius = 200.0f;

    bool isPlayerNearTheMouse = m_tileMapPaintOnMap.IsNearTheMouse(m_entity[player].GetTransform()->GetPosition(),
        m_entity[player].GetSprite()->GetWidthHeight() / 2, radius);

    if (isPlayerNearTheMouse)
    {
        bool isDrawOnMapAvalibleForPlayer = m_tileMapPaintOnMap.IsLeftMouseDown() && m_bIsWindowHovered;
        if (isDrawOnMapAvalibleForPlayer)
        {
            int seedNum = m_entity[player].GetInventory()->GetActiveSeedPacket();
            std::string seedString = m_entity[player].GetInventory()->GetName();
            int entityNum = m_entityController.GetEntityEnemyNumFromName(seedString);

            std::string texture = m_entity[player].GetInventory()->GetTexture();

            int spawnPos = m_tileMapPaintOnMap.GetTileMapPos();

            m_entity[player].GetInventory()->GetActiveSeedPacketCount();

            bool isTilePlantable =
                !m_tileMapLoader.GetTileTypeName(0, spawnPos).contains("Concrete") &&
                !m_tileMapLoader.GetTileTypeName(1, spawnPos).contains("Concrete") &&
                !m_tileMapLoader.GetTileTypeName(0, spawnPos).contains("Wood") &&
                !m_tileMapLoader.GetTileTypeName(1, spawnPos).contains("Wood") &&
                !m_tileMapLoader.GetTileTypeName(drawLayer, spawnPos).contains("transparent") &&
                m_tileMapLoader.GetTileTypeName(drawLayer, spawnPos) != "EmptyPlot" &&
                !m_entitySpawner.IsEntityPosTaken(spawnPos) &&
                m_entity[player].GetInventory()->GetActiveSeedPacketCount() > 0;

            if (isTilePlantable && !m_entitySpawner.IsPhaseNight())
            {
                m_tileMapLoader.UpdateTileType(drawLayer, spawnPos, m_entity[player].GetInventory()->GetName());
                m_tileMapDrawLayers[drawLayer][spawnPos].GetSprite()->UpdateTex(m_gfx->GetDevice(), texture);

                Vector2f offset = m_entityController.GetEnemyWidthHeight(entityNum) / 2;
                Vector2f spawnMapPos = m_tileMapPaintOnMap.GetMapPos(m_entity[player].GetTransform()->GetPosition(), offset);
                m_entitySpawner.AddEntityToSpawn(entityNum, spawnPos, spawnMapPos);

                std::pair<std::string, int>* seedattempt = new std::pair<std::string, int>();
                seedattempt->first = m_entity[player].GetInventory()->GetName();
                seedattempt->second = 1;
                EventSystem::Instance()->AddEvent(EVENTID::PlantSeedAttempt, seedattempt);
            }
        }
    }

    const float spawnTimmer = 0.2f;
    if (m_entitySpawner.IsPhaseNight())
    { 
        static float timmer = spawnTimmer;
        timmer += dt;

        if (m_entitySpawner.GetSpawnEntitiesSize() != 0 && timmer > spawnTimmer)
        {
            timmer -= spawnTimmer;
            static int count = 0;

            if (count == 0)
            {
                float* shootingDelay = new float;
                *shootingDelay = spawnTimmer * m_entitySpawner.GetSpawnEntitiesSize();
                EventSystem::Instance()->AddEvent(EVENTID::ShootingDelay, shootingDelay);
            }

            std::string texture = "Resources\\Textures\\Tiles\\EmptyPlot.png";
            int spawnPos = m_entitySpawner.GetSpawnEntitiesTileMapPos(count);
            m_tileMapLoader.UpdateTileType(drawLayer, spawnPos, "EmptyPlot");
            m_tileMapDrawLayers[1][spawnPos].GetSprite()->UpdateTex(m_gfx->GetDevice(), texture);

            m_entitySpawner.SpawnEntity(count);

            m_entityController.AddEntityData(m_entitySpawner.GetEntityData()[count]);
            count++;

            if (count >= m_entitySpawner.GetSpawnEntitiesSize())
            {
                count = 0;
                m_entitySpawner.EntitiesAdded();
            }
        }

        bool isAllEnemiesDead = m_entitySpawner.GetSpawnEntitiesSize() == 0 && *m_fCurrentHealth < 1;
        if (isAllEnemiesDead && m_entitySpawner.IsPhaseNight())
        {
            CleanUpEntities();
            EventSystem::Instance()->AddEvent(EVENTID::ChangePhase);
        }
    }
}

void Level::UpdateBothTileMaps(const float dt)
{
#if _DEBUG
    if (m_tileMapEditor.IsLayerSwitched())
#endif
    {
        for (int layer = 0; layer < m_tileMapDrawLayers.size(); layer++)
        {
            for (int pos = 0; pos < m_iTileMapRows * m_iTileMapColumns; pos++)
            {
                m_tileMapDrawLayers[layer][pos].Update(dt);

                std::string texture = "Resources\\Textures\\Tiles\\";
                std::string tileType = m_tileMapLoader.GetTileTypeName(layer, pos);
                texture += tileType;
                texture += ".png";

                m_tileMapDrawLayers[layer][pos].GetSprite()->UpdateTex(m_gfx->GetDevice(), texture);
            }
        }
#if _DEBUG
        m_tileMapEditor.SetLayerSwitchedDone();
#endif
    }
}

void Level::UpdateTileMapTexture(const float dt)
{
    int layer = m_tileMapEditor.GetTileMapLayerInt();

    if (m_tileMapEditor.IsLayerSwitched() || m_tileMapEditor.IsLoadedFile())
    {
        for (int pos = 0; pos < m_iTileMapRows * m_iTileMapColumns; pos++)
        {
            m_tileMapDrawLayers[layer][pos].Update(dt);

            std::string texture = "Resources\\Textures\\Tiles\\";
            texture += m_tileMapLoader.GetTileTypeName(layer, pos);
            texture += ".png";

            m_tileMapDrawLayers[layer][pos].GetSprite()->UpdateTex(m_gfx->GetDevice(), texture);
        }

        m_tileMapEditor.SetLoadedFileDone();
    }
    else
    {
        std::vector<int> updatedTiles = m_tileMapEditor.GetUpdatedTileMapTiles();

        for (int pos = 0; pos < updatedTiles.size(); pos++)
        {
            m_tileMapDrawLayers[layer][updatedTiles[pos]].Update(dt);

            std::string texture = "Resources\\Textures\\Tiles\\";
            texture += m_tileMapLoader.GetTileTypeName(layer, updatedTiles[pos]);
            texture += ".png";

            m_tileMapDrawLayers[layer][updatedTiles[pos]].GetSprite()->UpdateTex(m_gfx->GetDevice(), texture);
        }

        m_tileMapEditor.SetClearUpdatedTileMapTiles();
    }
}

void Level::UpdateTileMapEmpty(const float dt)
{
    if (m_tileMapEditor.IsLayerSwitched())
    {
        int layer = m_tileMapEditor.GetTileMapOtherLayerInt();
        for (int pos = 0; pos < m_iTileMapRows * m_iTileMapColumns; pos++)
        {
            m_tileMapDrawLayers[layer][pos].Update(dt);

            std::string texture = "Resources\\Textures\\Tiles\\transparent.png";

            m_tileMapDrawLayers[layer][pos].GetSprite()->UpdateTex(m_gfx->GetDevice(), texture);
        }

        m_tileMapEditor.SetLayerSwitchedDone();
    }
}

void Level::AddToEvent() noexcept
{
    EventSystem::Instance()->AddClient(EVENTID::GamePauseEvent, this);
    EventSystem::Instance()->AddClient(EVENTID::GameUnpauseEvent, this);
    EventSystem::Instance()->AddClient(EVENTID::FinalNight, this);
}

void Level::RemoveFromEvent() noexcept
{
    EventSystem::Instance()->RemoveClient(EVENTID::GamePauseEvent, this);
    EventSystem::Instance()->RemoveClient(EVENTID::GameUnpauseEvent, this);
    EventSystem::Instance()->RemoveClient(EVENTID::FinalNight, this);
}

void Level::HandleEvent(Event* event)
{
    // Switch level
    switch (event->GetEventID())
    {
    case EVENTID::GamePauseEvent:
    {
        m_bIsGamePaused = true;
    }
    break;
    case EVENTID::GameUnpauseEvent:
    {
        m_bIsGamePaused = false;
    }
    case EVENTID::FinalNight:
    {
        SpawnFinalBoss();
        break;
    }
    break;
    }
}