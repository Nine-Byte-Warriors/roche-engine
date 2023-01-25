#include "stdafx.h"
#include "Level1.h"
#include "ProjectileEditor.h"

#if _DEBUG
#include <imgui/imgui.h>
#endif

void Level1::OnCreate()
{
	try
	{
		// Initialize constant buffers
		HRESULT hr = m_cbMatrices.Initialize( m_gfx->GetDevice(), m_gfx->GetContext() );
		COM_ERROR_IF_FAILED( hr, "Failed to create 'Matrices' constant buffer!" );

        OnCreateEntity();

        // Initialize 2d camera
        XMFLOAT2 aspectRatio = { static_cast<float>( m_gfx->GetWidth() ), static_cast<float>( m_gfx->GetHeight() ) };
        m_camera.SetProjectionValues( aspectRatio.x, aspectRatio.y, 0.0f, 1.0f );

        // Initialize systems
        m_textRenderer.Initialize( "beth_ellen_ms_16_bold.spritefont", m_gfx->GetDevice(), m_gfx->GetContext() );

        // Initialize TileMap
        OnCreateTileMap();     

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

void Level1::OnCreateEntity()
{
    m_iEntityAmount = m_entityController.GetSize();
    for (int i = 0; i < m_iEntityAmount; i++)
    {
        Entity *entityPop = new Entity(m_entityController, i);
        m_entity.push_back(*entityPop);

        m_entity[i].Initialize(*m_gfx, m_cbMatrices);

        delete entityPop;
    }

    for (int i = 0; i < m_iEntityAmount; i++)
    {
        m_collisionHandler.AddCollider(m_entity[i].GetCollider());
    }
}

void Level1::OnCreateTileMap()
{
    m_iTileMapRows = (m_gfx->GetHeight() / m_iTileSize) + 1;
    m_iTileMapColumns = m_gfx->GetWidth() / m_iTileSize;
    m_tileMapLoader = new TileMapLoader(m_iTileMapRows, m_iTileMapColumns);

#ifdef _DEBUG
    m_tileMapEditor = new TileMapEditor(m_iTileMapRows, m_iTileMapColumns);
    m_tileMapLoader->SetLevel(m_tileMapEditor->GetLevel(TileMapLayer::Background), m_tileMapEditor->GetLevel(TileMapLayer::Foreground));
#else
    m_tileMapLoader->LoadLevel("Resources\\TileMaps\\blue.json", "Resources\\TileMaps\\blue.json");
#endif  

    OnCreateTileMapDraw();
}

void Level1::OnCreateTileMapDraw()
{
    int colPositionTotalTileLength = 0;
    int rowPositionTotalTileLength = 0;
    const int gapBetweenTiles = 0;
    
    for (int i = 0; i < m_iTileMapLayers; i++)
    {
        std::vector<TileMapDraw>* tileMapDraw = new std::vector<TileMapDraw>;

        for (int j = 0; j < m_iTileMapRows * m_iTileMapColumns; j++)
        {
            TileMapDraw* tileMapDrawPop = new TileMapDraw;
            tileMapDraw->push_back(*tileMapDrawPop);
            (*tileMapDraw)[j].Initialize(*m_gfx, m_cbMatrices, "NONE");

            if (j != 0)
            {
                colPositionTotalTileLength += m_iTileSize + gapBetweenTiles;
            }
            bool endOfRow = j % m_iTileMapColumns == 0 && j != 0;
            if (endOfRow)
            {
                rowPositionTotalTileLength += m_iTileSize + gapBetweenTiles;
                colPositionTotalTileLength = 0;
            }

            float positionWidth = colPositionTotalTileLength;
            float positionHeight = rowPositionTotalTileLength;

            (*tileMapDraw)[j].GetTransform()->SetPositionInit(positionWidth, positionHeight);
            (*tileMapDraw)[j].GetTransform()->SetScaleInit(m_iTileSize, m_iTileSize);

            delete tileMapDrawPop;
        }

        m_tileMapDrawLayers.push_back(*tileMapDraw);
        tileMapDraw->clear();
        delete tileMapDraw;

        colPositionTotalTileLength = 0;
        rowPositionTotalTileLength = 0;
    }
}

void Level1::OnSwitch()
{
	// Update level system
	CurrentLevel = 0;
	levelName = "Level1";
	EventSystem::Instance()->AddEvent( EVENTID::SetCurrentLevelEvent, &CurrentLevel );
	NextLevel = 1;
	EventSystem::Instance()->AddEvent( EVENTID::SetNextLevelEvent, &NextLevel );

    // Update user interface
    EventSystem::Instance()->AddEvent( EVENTID::ShowCursorEvent );
    m_ui->RemoveAllUI();
    for ( unsigned int i = 0; i < m_uiEditor.GetScreens().size(); i++ )
	    m_ui->AddUI( m_uiEditor.GetScreens()[i], m_uiEditor.GetScreenData()[i].name );
	m_ui->Initialize( *m_gfx, &m_cbMatrices, m_uiEditor.GetWidgets() );
    m_ui->HideAllUI();

#if !_DEBUG
    m_ui->ShowUI( "Pause" );
#endif
}

void Level1::BeginFrame()
{
	// Setup pipeline state
	m_gfx->BeginFrame();
	m_gfx->UpdateRenderState();
}

void Level1::RenderFrame()
{
	auto gfxContext = m_gfx->GetContext();
	auto camMatrix = m_camera.GetWorldOrthoMatrix();
    
    // Sprites
    RenderFrameTileMap();
    
    m_projectileEditor->Draw(gfxContext, camMatrix);
    
    RenderFrameEntity();
}

void Level1::RenderFrameEntity()
{
    for (int i = 0; i < m_iEntityAmount; i++)
    {
        m_entity[i].GetSprite()->UpdateBuffers(m_gfx->GetContext());
        m_entity[i].GetSprite()->Draw(m_entity[i].GetTransform()->GetWorldMatrix(), m_camera.GetWorldOrthoMatrix());

        if (m_entity[i].GetEntityType() == EntityType::Player)
        {
            m_entity[i].GetProjectileManager()->Draw(m_gfx->GetContext(), m_camera.GetWorldOrthoMatrix());
        }
    }
}

void Level1::RenderFrameTileMap()
{
    for (int i = 0; i < m_tileMapDrawLayers.size(); i++)
    {
        for (unsigned j = 0; j < m_iTileMapRows * m_iTileMapColumns; j++)
        {
            m_tileMapDrawLayers[i][j].GetSprite()->UpdateBuffers(m_gfx->GetContext());
            m_tileMapDrawLayers[i][j].GetSprite()->Draw(m_tileMapDrawLayers[i][j].GetTransform()->GetWorldMatrix(), m_camera.GetWorldOrthoMatrix());
        }
    }
}

void Level1::EndFrame()
{
    // Render ui
    m_ui->Draw(
        m_gfx->GetShaderVtx(), m_gfx->GetShaderPix(),
        m_camera.GetWorldOrthoMatrix(), &m_textRenderer
    );

    // Render scene to texture
    m_gfx->RenderSceneToTexture();

#if _DEBUG
    // Render imgui windows
    m_imgui->BeginRender();

    ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 0.0f, 0.0f ) );
    if ( ImGui::Begin( "Scene Window", FALSE ) )
    {
        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImVec2 vMax = ImGui::GetWindowContentRegionMax();
        
        // Update imgui mouse position for scene render window
        Vector2f* mousePos = new Vector2f( ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y );
        EventSystem::Instance()->AddEvent( EVENTID::ImGuiMousePosition, mousePos );

        vMax.x += ImGui::GetWindowPos().x;
        vMax.y += ImGui::GetWindowPos().y;

        ImGui::GetWindowDrawList()->AddImage(
            (void*)m_gfx->GetRenderTargetPP()->GetShaderResourceView(),
            pos, ImVec2( vMax.x, vMax.y )
        );
    }
    ImGui::End();
    ImGui::PopStyleVar();

    m_imgui->SpawnInstructionWindow();
    m_gfx->SpawnControlWindow();
	
    Vector2f GOpos = m_enemy.GetTransform()->GetPosition();
    Vector2f Tpos = m_enemy.GetAI()->GetTargetPosition();
    m_enemy.GetAI()->SpawnControlWindow(GOpos, Tpos);

    m_uiEditor.SpawnControlWindow( *m_gfx );
    m_projectileEditor->SpawnEditorWindow(*m_gfx, m_cbMatrices);
    m_tileMapEditor->SpawnControlWindow();
    m_entityEditor.SpawnControlWindow(m_gfx->GetWidth(), m_gfx->GetHeight());
    m_audioEditor.SpawnControlWindow();
    m_player.SpawnControlWindow();
    m_imgui->EndRender();
#endif
    
    // Present Frame
	m_gfx->EndFrame();
}

void Level1::Update( const float dt )
{
#if _DEBUG
    m_uiEditor.Update( dt );
    static bool firstLoad = true;
    if ( m_uiEditor.ShouldShowAll() || firstLoad )
    {
        firstLoad = false;
        m_ui->ShowAllUI();
    }
    else if ( m_uiEditor.GetCurrentScreenIndex() > -1 )
    {
        m_ui->HideAllUI();
        std::string name = m_uiEditor.GetCurrentScreenName();
        m_ui->ShowUI( m_uiEditor.GetCurrentScreenName() );
    }
    else
    {
        m_ui->HideAllUI();
    }
#endif
    UpdateTileMap( dt );

    UpdateEntity( dt );

    m_ui->Update( dt, m_uiEditor.GetWidgets() );

	m_projectileEditor->Update( dt );
    m_collisionHandler.Update();
}

void Level1::UpdateEntity(const float dt)
{
#if _DEBUG
    UpdateEntityFromEditor(dt);
#endif

    for (int i = 0; i < m_iEntityAmount; i++)
    {
        m_entity[i].Update(dt);
    }
}

void Level1::UpdateEntityFromEditor(const float dt)
{
    m_entityController.SetEntityData(m_entityEditor.GetEntityData());

    if (m_iEntityAmount < m_entityController.GetSize())
    {
        for (int i = m_iEntityAmount; i < m_entityController.GetSize(); i++)
        {
            Entity* entityPop = new Entity(m_entityController, i);
            m_entity.push_back(*entityPop);
            m_entity[i].Initialize(*m_gfx, m_cbMatrices);
            delete entityPop;

            m_entity[i].GetSprite()->UpdateBuffers(m_gfx->GetContext());
            m_entity[i].GetSprite()->Draw(m_entity[i].GetTransform()->GetWorldMatrix(), m_camera.GetWorldOrthoMatrix());
            m_entity[i].GetProjectileManager()->Draw(m_gfx->GetContext(), m_camera.GetWorldOrthoMatrix());
        }
        m_iEntityAmount = m_entityEditor.GetEntityData().size();
    }

    for (int i = 0; i < m_iEntityAmount; i++)
    {
        m_entity[i].UpdateFromEntityData(dt, m_entityEditor.IsPositionLocked());
    }
}

void Level1::UpdateTileMap(const float dt)
{
#if _DEBUG
    if (m_tileMapEditor->IsDrawOnceAvalible() || m_tileMapEditor->IsDrawContinuousAvalible())
    {
        if (m_tileMapEditor->GetTileMapLayer() != TileMapLayer::Both)
        {
            UpdateTileMapTexture(dt);
            UpdateTileMapEmpty(dt);
        }
        else
        {
            UpdateBothTileMaps(dt);
        }

        m_tileMapEditor->SetDrawOnceDone();
    }

#else
    if (m_bMapUpdate)
    {
        UpdateBothTileMaps(dt);
        m_bMapUpdate = false;
    }
#endif
}

void Level1::UpdateBothTileMaps(const float dt)
{
#if _DEBUG
    if (m_tileMapEditor->IsLayerSwitched())
#endif
    {
        for (int layer = 0; layer < m_tileMapDrawLayers.size(); layer++)
        {
            for (int pos = 0; pos < m_iTileMapRows * m_iTileMapColumns; pos++)
            {
                m_tileMapDrawLayers[layer][pos].Update(dt);

                std::string texture = "Resources\\Textures\\Tiles\\";
                texture += m_tileMapLoader->GetTileTypeName(layer, pos);
                texture += ".png";

                m_tileMapDrawLayers[layer][pos].GetSprite()->UpdateTex(m_gfx->GetDevice(), texture);
            }
        }
#if _DEBUG
        m_tileMapEditor->SetLayerSwitchedDone();
#endif
    }
}

void Level1::UpdateTileMapTexture(const float dt)
{
    int layer = m_tileMapEditor->GetTileMapLayerInt();

    if (m_tileMapEditor->IsLayerSwitched() || m_tileMapEditor->IsLoadedFile())
    {
        for (int pos = 0; pos < m_iTileMapRows * m_iTileMapColumns; pos++)
        {
            m_tileMapDrawLayers[layer][pos].Update(dt);

            std::string texture = "Resources\\Textures\\Tiles\\";
            texture += m_tileMapLoader->GetTileTypeName(layer, pos);
            texture += ".png";

            m_tileMapDrawLayers[layer][pos].GetSprite()->UpdateTex(m_gfx->GetDevice(), texture);
        }

        m_tileMapEditor->SetLoadedFileDone();
    }
    else
    {
        std::vector<int> updatedTiles = m_tileMapEditor->GetUpdatedTileMapTiles();

        for (int pos = 0; pos < updatedTiles.size(); pos++)
        {
            m_tileMapDrawLayers[layer][updatedTiles[pos]].Update(dt);

            std::string texture = "Resources\\Textures\\Tiles\\";
            texture += m_tileMapLoader->GetTileTypeName(layer, updatedTiles[pos]);
            texture += ".png";

            m_tileMapDrawLayers[layer][updatedTiles[pos]].GetSprite()->UpdateTex(m_gfx->GetDevice(), texture);
        }

        m_tileMapEditor->SetClearUpdatedTileMapTiles();
    }
}

void Level1::UpdateTileMapEmpty(const float dt)
{
    if (m_tileMapEditor->IsLayerSwitched())
    {
        int layer = m_tileMapEditor->GetTileMapOtherLayerInt();
        for (int pos = 0; pos < m_iTileMapRows * m_iTileMapColumns; pos++)
        {
            m_tileMapDrawLayers[layer][pos].Update(dt);

            std::string texture = "Resources\\Textures\\Tiles\\transparent.png";

            m_tileMapDrawLayers[layer][pos].GetSprite()->UpdateTex(m_gfx->GetDevice(), texture);
        }

        m_tileMapEditor->SetLayerSwitchedDone();
    }
}

void Level1::CleanUp()
{
    delete m_tileMapEditor;
    delete m_tileMapLoader;
}