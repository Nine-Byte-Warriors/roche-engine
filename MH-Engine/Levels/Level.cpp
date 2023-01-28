#include "stdafx.h"
#include "Level.h"
#include "ProjectileEditor.h"

#if _DEBUG
#include <imgui/imgui.h>
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
        m_textRenderer.Initialize( "beth_ellen_ms_16_bold.spritefont", m_gfx->GetDevice(), m_gfx->GetContext() );

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
    m_entity.clear();
    m_iEntityAmount = m_entityController.GetSize();
    for (int i = 0; i < m_iEntityAmount; i++)
    {
        Entity *entityPop = new Entity(m_entityController, i);
        m_entity.push_back(*entityPop);
        m_entity[i].Initialize(*m_gfx, m_cbMatrices);
        delete entityPop;
    }

    m_collisionHandler.RemoveAllColliders();
    for (int i = 0; i < m_iEntityAmount; i++)
    {
        if (m_entityController.HasCollider(i))
        {
            m_collisionHandler.AddCollider(m_entity[i].GetCollider());
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
	m_ui->Initialize( *m_gfx, &m_cbMatrices, m_uiEditor.GetWidgets() );
    m_ui->HideAllUI();

#if !_DEBUG
    m_ui->ShowUI( "Credits" );
#endif
}

void Level::CreateTileMap()
{
    m_iTileMapRows = (m_gfx->GetHeight() / m_iTileSize) + 1;
    m_iTileMapColumns = m_gfx->GetWidth() / m_iTileSize;
    m_tileMapLoader.Initialize(m_iTileMapRows, m_iTileMapColumns);

#ifdef _DEBUG
    m_tileMapEditor.Initialize(m_iTileMapRows, m_iTileMapColumns);
    m_tileMapLoader.SetLevel(m_tileMapEditor.GetLevel(TileMapLayer::Background), m_tileMapEditor.GetLevel(TileMapLayer::Foreground));
#else
    m_tileMapLoader.LoadLevel("blue.json", "blue.json");
#endif

    CreateTileMapDraw();
}

void Level::CreateTileMapDraw()
{
    int colPositionTotalTileLength = 0;
    int rowPositionTotalTileLength = 0;
    const int gapBetweenTiles = 0;

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
                colPositionTotalTileLength = 0;
            }

            float positionWidth = colPositionTotalTileLength;
            float positionHeight = rowPositionTotalTileLength;

            (*tileMapDraw)[j].GetTransform()->SetPositionInit(positionWidth, positionHeight);
            (*tileMapDraw)[j].GetTransform()->SetScaleInit(m_iTileSize, m_iTileSize);
        }

        m_tileMapDrawLayers.push_back(*tileMapDraw);
        tileMapDraw->clear();
        delete tileMapDraw;

        colPositionTotalTileLength = 0;
        rowPositionTotalTileLength = 0;
    }
}

void Level::OnSwitch()
{
	// Update level system
	EventSystem::Instance()->AddEvent( EVENTID::SetCurrentLevelEvent, &m_iCurrentLevel );
	EventSystem::Instance()->AddEvent( EVENTID::SetNextLevelEvent, &m_iNextLevel );
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
    for (int i = 0; i < m_iEntityAmount; i++)
    {
        m_entity[i].GetSprite()->UpdateBuffers(m_gfx->GetContext());
        m_entity[i].GetSprite()->Draw(m_entity[i].GetTransform()->GetWorldMatrix(), m_camera.GetWorldOrthoMatrix());

        if (m_entityController.HasProjectileBullet(i))
        {
            m_entity[i].GetProjectileManager()->Draw(m_gfx->GetContext(), m_camera.GetWorldOrthoMatrix());
        }
    }
}

void Level::RenderFrameTileMap()
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

void Level::EndFrame_Start()
{
    // Render ui
    m_ui->Draw(
        m_gfx->GetShaderVtx(), m_gfx->GetShaderPix(),
        m_camera.GetWorldOrthoMatrix(), &m_textRenderer );

    // Render scene to texture
    m_gfx->RenderSceneToTexture();

#if _DEBUG
    // Render imgui windows
    m_imgui->BeginRender();
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
            std::string sFakedMouseText = "Faked Mouse Pos: "
                " X: " + std::to_string(vFakedPos->x) +
                " Y: " + std::to_string(vFakedPos->y);
            ImGui::Text(sFakedMouseText.c_str());
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
#endif
}

void Level::EndFrame_End()
{
#if _DEBUG
    m_imgui->EndRender();
#endif

    // Present Frame
	m_gfx->EndFrame();
}

void Level::Update( const float dt )
{
    UpdateTileMap( dt );
    UpdateEntity( dt );
    UpdateUI( dt );

	m_projectileEditor->Update( dt );
    m_collisionHandler.Update();
    m_camera.Update( dt );
}

void Level::UpdateEntity(const float dt)
{
#if _DEBUG
    UpdateEntityFromEditor(dt);
#endif

    for (int i = 0; i < m_iEntityAmount; i++)
    {
        m_entity[i].Update(dt);
    }
}

void Level::UpdateUI( const float dt )
{
#if _DEBUG
    if ( m_uiEditor.GetShouldUpdate() )
    {
        // Update user interface
        m_ui->RemoveAllUI();
        for ( unsigned int i = 0; i < m_uiEditor.GetScreens().size(); i++ )
	        m_ui->AddUI( m_uiEditor.GetScreens()[i], m_uiEditor.GetScreenData()[i].name );
	    m_ui->Initialize( *m_gfx, &m_cbMatrices, m_uiEditor.GetWidgets() );
        m_ui->HideAllUI();

#if !_DEBUG
        m_ui->ShowUI( "Pause" );
#endif
        m_uiEditor.SetShouldUpdate( false );
    }

    m_uiEditor.Update( dt );
    static bool firstLoadEver = true;
    if ( m_uiEditor.ShouldShowAll() || firstLoadEver )
    {
        m_ui->ShowAllUI();
        firstLoadEver = false;
    }
    else if ( m_uiEditor.GetCurrentScreenIndex() > -1 )
    {
        m_ui->HideAllUI();
        m_ui->ShowUI( m_uiEditor.GetCurrentScreenName() );
    }
    else
    {
        m_ui->HideAllUI();
    }
    if ( m_uiEditor.ShouldHideAll() )
    {
        m_ui->HideAllUI();
    }
#endif
    m_ui->Update( dt, m_uiEditor.GetWidgets() );
}

void Level::UpdateEntityFromEditor(const float dt)
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

            if (m_entityController.HasCollider(i))
            {
                m_collisionHandler.AddCollider(m_entity[i].GetCollider());
            }

            m_entity[i].GetSprite()->UpdateBuffers(m_gfx->GetContext());
            m_entity[i].GetSprite()->Draw(m_entity[i].GetTransform()->GetWorldMatrix(), m_camera.GetWorldOrthoMatrix());

            if (m_entityController.HasProjectileSystem(i))
            {
                m_entity[i].GetProjectileManager()->Draw(m_gfx->GetContext(), m_camera.GetWorldOrthoMatrix());
            }
        }
        m_iEntityAmount = m_entityEditor.GetEntityData().size();

        m_entityController.UpdateCopy();
    }
    else if (m_iEntityAmount != m_entityController.GetSize() || m_entityController.HasComponentUpdated())
    {
        m_entity.clear();
        m_collisionHandler.RemoveAllColliders();

        for (int i = 0; i < m_entityController.GetSize(); i++)
        {
            Entity* entityPop = new Entity(m_entityController, i);
            m_entity.push_back(*entityPop);
            m_entity[i].Initialize(*m_gfx, m_cbMatrices);
            delete entityPop;

            if (m_entityController.HasCollider(i))
            {
                m_collisionHandler.AddCollider(m_entity[i].GetCollider());
            }

            m_entity[i].GetSprite()->UpdateBuffers(m_gfx->GetContext());
            m_entity[i].GetSprite()->Draw(m_entity[i].GetTransform()->GetWorldMatrix(), m_camera.GetWorldOrthoMatrix());

            if (m_entityController.HasProjectileSystem(i))
            {
                m_entity[i].GetProjectileManager()->Draw(m_gfx->GetContext(), m_camera.GetWorldOrthoMatrix());
            }
        }
        m_iEntityAmount = m_entityEditor.GetEntityData().size();

        m_entityController.UpdateCopy();
    }

    for (int i = 0; i < m_iEntityAmount; i++)
    {
        m_entity[i].UpdateFromEntityData(dt, m_entityEditor.IsPositionLocked());
    }
}

void Level::UpdateTileMap(const float dt)
{
#if _DEBUG
    if (m_tileMapEditor.IsDrawOnceAvalible() || m_tileMapEditor.IsDrawContinuousAvalible())
    {
        if (m_tileMapEditor.GetTileMapLayer() != TileMapLayer::Both)
        {
            UpdateTileMapTexture(dt);
            UpdateTileMapEmpty(dt);
        }
        else
        {
            UpdateBothTileMaps(dt);
        }

        m_tileMapEditor.SetDrawOnceDone();
    }

#else
    if (m_bMapUpdate)
    {
        UpdateBothTileMaps(dt);
        m_bMapUpdate = false;
    }
#endif
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