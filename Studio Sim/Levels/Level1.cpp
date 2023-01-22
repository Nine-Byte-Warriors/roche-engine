#include "stdafx.h"
#include "Level1.h"
#include "ProjectileEditor.h"

#if _DEBUG
#include <imgui/imgui.h>
#endif

#define DEBUG_ENEMY_COUNT 5

void Level1::OnCreate()
{
	try
	{
		// Initialize constant buffers
		HRESULT hr = m_cbMatrices.Initialize( m_gfx->GetDevice(), m_gfx->GetContext() );
		COM_ERROR_IF_FAILED( hr, "Failed to create 'Matrices' constant buffer!" );

        // Initialize player
        m_player.Initialize( *m_gfx, m_cbMatrices );
        m_player.GetTransform()->SetPositionInit( m_gfx->GetWidth() * 0.55f, m_gfx->GetHeight() / 2 );
        m_player.GetTransform()->SetScaleInit( m_player.GetSprite()->GetWidth(), m_player.GetSprite()->GetHeight() );

        OnCreateEntity();

        // Initialize enemies
        m_enemy.Initialize( *m_gfx, m_cbMatrices, Sprite::Type::Tomato );
        
        for (int i = 0; i < DEBUG_ENEMY_COUNT; i++)
        {
		    std::shared_ptr<Enemy> pEnemy = std::make_shared<Enemy>();
            // TODO: set ai states and initial positions here for each enemy
		    m_enemyManager.AddEnemy(std::move(pEnemy));
        }
        m_enemyManager.Initialize(*m_gfx, m_cbMatrices, Sprite::Type::Cauliflower);

        // Initialize 2d camera
        XMFLOAT2 aspectRatio = { static_cast<float>( m_gfx->GetWidth() ), static_cast<float>( m_gfx->GetHeight() ) };
        m_camera.SetProjectionValues( aspectRatio.x, aspectRatio.y, 0.0f, 1.0f );

        // Initialize systems
        m_textRenderer.Initialize( "beth_ellen_ms_16_bold.spritefont", m_gfx->GetDevice(), m_gfx->GetContext() );

        // Initialize TileMap
        OnCreateTileMap(m_tileMapDrawBackground);
        OnCreateTileMap(m_tileMapDrawForeground);

        //Initialize CollisionHandler
        m_collisionHandler.AddCollider(m_player.GetCollider());
        m_collisionHandler.AddCollider(m_enemy.GetCollider());

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

void Level1::OnCreateTileMap(std::vector<TileMapDraw>& tileMapDraw)
{
    int colPositionTotalTileLength = 0;
    int rowPositionTotalTileLength = 0;
    const int tileSize = 32;
    const int gapBetweenTiles = 0;
    m_iTileMapRows = ( m_gfx->GetHeight() / tileSize ) + 1;
    m_iTileMapColumns = m_gfx->GetWidth() / tileSize;

    m_tileMapEditor = new TileMapEditor(m_iTileMapRows, m_iTileMapColumns);

    for (int i = 0; i < m_iTileMapRows * m_iTileMapColumns; i++)
    {
        TileMapDraw *tileMapDrawPop = new TileMapDraw;
        tileMapDraw.push_back(*tileMapDrawPop);
        tileMapDraw[i].Initialize(*m_gfx, m_cbMatrices, "NONE");

        if (i != 0)
        {
            colPositionTotalTileLength += tileSize + gapBetweenTiles;
        }
        bool endOfRow = i % m_iTileMapColumns == 0 && i != 0;
        if (endOfRow)
        {
            rowPositionTotalTileLength += tileSize + gapBetweenTiles;
            colPositionTotalTileLength = 0;
        }

        float positionWidth = colPositionTotalTileLength;
        float positionHeight = rowPositionTotalTileLength;

        tileMapDraw[i].GetTransform()->SetPositionInit(positionWidth, positionHeight);
        tileMapDraw[i].GetTransform()->SetScaleInit(tileSize, tileSize);

        delete tileMapDrawPop;
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
    RenderFrameTileMap(m_tileMapDrawBackground);
    RenderFrameTileMap(m_tileMapDrawForeground);
    
    m_projectileEditor->Draw(gfxContext, camMatrix);

    m_player.GetSprite()->UpdateBuffers(gfxContext);
    m_player.GetSprite()->Draw( m_player.GetTransform()->GetWorldMatrix(), camMatrix);
    m_player.GetProjectileManager()->Draw(gfxContext, camMatrix);
    
    RenderFrameEntity();

	m_enemy.Render(*m_gfx, camMatrix);
	m_enemyManager.Render(*m_gfx, camMatrix);
}

void Level1::RenderFrameEntity()
{
    for (int i = 0; i < m_iEntityAmount; i++)
    {
        m_entity[i].GetSprite()->UpdateBuffers(m_gfx->GetContext());
        m_entity[i].GetSprite()->Draw(m_entity[i].GetTransform()->GetWorldMatrix(), m_camera.GetWorldOrthoMatrix());
        m_entity[i].GetProjectileManager()->Draw(m_gfx->GetContext(), m_camera.GetWorldOrthoMatrix());
    }
}

void Level1::RenderFrameTileMap(std::vector<TileMapDraw>& tileMapDraw)
{
    for ( unsigned i = 0; i < m_iTileMapRows * m_iTileMapColumns; i++ )
    {
        tileMapDraw[i].GetSprite()->UpdateBuffers(m_gfx->GetContext());
        tileMapDraw[i].GetSprite()->Draw(tileMapDraw[i].GetTransform()->GetWorldMatrix(), m_camera.GetWorldOrthoMatrix());
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
    // Update entities
#if _DEBUG
    m_audioEditor.Update();
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
    UpdateTileMap( dt, m_tileMapDrawBackground, TileMapLayer::Background);
    UpdateTileMap( dt, m_tileMapDrawForeground, TileMapLayer::Foreground);

    UpdateEntity( dt );

    m_player.Update( dt );
    m_enemy.Update( dt );
	m_enemyManager.Update( dt );

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
        m_entity[i].UpdateFromEntityData(dt);
    }
}

void Level1::UpdateTileMap(const float dt, std::vector<TileMapDraw>& tileMapDraw, TileMapLayer tileMapLayer)
{
    const int numberOfTileMapLayers = 2;
    static int firstTimeTileMapDrawBothLayers = numberOfTileMapLayers;
#if _DEBUG
    static int updateBothTileMapLayers = numberOfTileMapLayers;

    if (m_tileMapEditor->UpdateDrawOnceAvalible() || firstTimeTileMapDrawBothLayers > 0 || m_tileMapEditor->UpdateDrawContinuousAvalible())
#else
    if (firstTimeTileMapDrawBothLayers > 0)
#endif
    {
#if _DEBUG
        if (tileMapLayer == m_tileMapEditor->GetTileMapLayer() || m_tileMapEditor->GetTileMapLayer() == TileMapLayer::Both)
#endif
        {
            for (int i = 0; i < m_iTileMapRows * m_iTileMapColumns; i++)
            {
                tileMapDraw[i].Update(dt);

                std::string texture = "Resources\\Textures\\Tiles\\";
                texture += m_tileMapEditor->GetTileTypeName(i, tileMapLayer);
                texture += ".png";

                tileMapDraw[i].GetSprite()->UpdateTex(m_gfx->GetDevice(), texture);
            }
        }
#if _DEBUG
        else
        {
            for (int i = 0; i < m_iTileMapRows * m_iTileMapColumns; i++)
            {
                tileMapDraw[i].Update(dt);

                std::string texture = "Resources\\Textures\\Tiles\\transparent.png";

                tileMapDraw[i].GetSprite()->UpdateTex(m_gfx->GetDevice(), texture);
            }
        }
        updateBothTileMapLayers--;
        if (updateBothTileMapLayers == 0)
        {
            m_tileMapEditor->UpdateDrawOnceDone();
            updateBothTileMapLayers = numberOfTileMapLayers;
        }
#endif

        firstTimeTileMapDrawBothLayers--;
    }
}

void Level1::CleanUp()
{
    delete m_tileMapEditor;
}