#include "stdafx.h"
#include "Level1.h"

#if _DEBUG
#include <imgui/imgui.h>
#endif

void Level1::OnCreate()
{
	try
	{
		// Initialize constant buffers
		HRESULT hr = m_cbMatrices.Initialize( graphics->GetDevice(), graphics->GetContext() );
		COM_ERROR_IF_FAILED( hr, "Failed to create 'Matrices' constant buffer!" );

        // Initialize player
        m_player.Initialize( *graphics, m_cbMatrices );
        m_player.GetTransform()->SetPositionInit( graphics->GetWidth() * 0.55f, graphics->GetHeight() / 2 );
        m_player.GetTransform()->SetScaleInit( m_player.GetSprite()->GetWidth(), m_player.GetSprite()->GetHeight() );

        // Initialize enemies
        m_enemy.Initialize( *graphics, m_cbMatrices, Sprite::Type::Tomato );
        m_enemy.GetTransform()->SetPositionInit( graphics->GetWidth() * 0.45f, graphics->GetHeight() / 2 );
        m_enemy.GetTransform()->SetScaleInit( m_enemy.GetSprite()->GetWidth(), m_enemy.GetSprite()->GetHeight() );

        // Initialize 2d camera
        XMFLOAT2 aspectRatio = { static_cast<float>( graphics->GetWidth() ), static_cast<float>( graphics->GetHeight() ) };
        m_camera.SetProjectionValues( aspectRatio.x, aspectRatio.y, 0.0f, 1.0f );

        // Initialize systems
        m_textRenderer.Initialize( "open_sans_ms_16_bold.spritefont", graphics->GetDevice(), graphics->GetContext() );

        // Initialize TileMap
        OnCreateTileMap();
	}
	catch ( COMException& exception )
	{
		ErrorLogger::Log( exception );
        return;
	}
}

void Level1::OnCreateTileMap()
{
    int colPositionTotalTileLength = 0;
    int rowPositionTotalTileLength = 0;

    for (int i = 0; i < COLUMNS * ROWS; i++)
    {
        const int gapBetweenTiles = 0;

        TileMapDraw *tileMapDrawPop = new TileMapDraw;
        m_tileMapDraw.push_back(*tileMapDrawPop);
        m_tileMapDraw[i].Initialize(*graphics, m_cbMatrices, "Resources\\Textures\\empty.png");

        if (i != 0)
        {
            colPositionTotalTileLength += m_tileMapDraw[i].GetSprite()->GetWidth() + gapBetweenTiles;
        }
        if (i % COLUMNS == 0 && i != 0)
        {
            rowPositionTotalTileLength += m_tileMapDraw[i].GetSprite()->GetHeight() + gapBetweenTiles;
            colPositionTotalTileLength = 0;
        }

        float positionWidth = colPositionTotalTileLength + (graphics->GetWidth() / 2) - (m_tileMapDraw[i].GetSprite()->GetWidth() * (COLUMNS / 2));
        float positionHeight = rowPositionTotalTileLength + (graphics->GetHeight() / 2) - (m_tileMapDraw[i].GetSprite()->GetHeight() * (ROWS / 2));

        m_tileMapDraw[i].GetTransform()->SetPositionInit(positionWidth, positionHeight);
        m_tileMapDraw[i].GetTransform()->SetScaleInit(m_tileMapDraw[i].GetSprite()->GetWidth(), m_tileMapDraw[i].GetSprite()->GetHeight());

        delete tileMapDrawPop;
    }
}

void Level1::OnSwitch()
{
	// Update Level System
	CurrentLevel = 1;
	levelName = "Level1";
	EventSystem::Instance()->AddEvent( EVENTID::SetCurrentLevelEvent, &CurrentLevel );
	NextLevel = 2;
	EventSystem::Instance()->AddEvent( EVENTID::SetNextLevelEvent, &NextLevel );
}

void Level1::BeginFrame()
{
	// Setup pipeline state
	graphics->BeginFrame();
	graphics->UpdateRenderState();
}

void Level1::RenderFrame()
{
    // Sprites
    RenderFrameTileMap();

    m_player.GetSprite()->UpdateBuffers( graphics->GetContext() );
    m_player.GetSprite()->Draw( m_player.GetTransform()->GetWorldMatrix(), m_camera.GetWorldOrthoMatrix() );

    m_enemy.GetSprite()->UpdateBuffers( graphics->GetContext() );
    m_enemy.GetSprite()->Draw( m_enemy.GetTransform()->GetWorldMatrix(), m_camera.GetWorldOrthoMatrix() );
}

void Level1::RenderFrameTileMap()
{
    for (int i = 0; i < COLUMNS * ROWS; i++)
    {
        m_tileMapDraw[i].GetSprite()->UpdateBuffers(graphics->GetContext());
        m_tileMapDraw[i].GetSprite()->Draw(m_tileMapDraw[i].GetTransform()->GetWorldMatrix(), m_camera.GetWorldOrthoMatrix());
    }
}

void Level1::EndFrame()
{
	// Render text
    m_textRenderer.RenderString(
        "This is example text.",
        XMFLOAT2( graphics->GetWidth() * 0.5f, graphics->GetHeight() * 0.96f ),
        Colors::Green, true
    );

    // Render scene to texture
    graphics->RenderSceneToTexture();

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
            (void*)graphics->GetRenderTargetPP()->GetShaderResourceView(),
            pos, ImVec2( vMax.x, vMax.y )
        );
    }
    ImGui::End();
    ImGui::PopStyleVar();

    m_imgui->SpawnInstructionWindow();
    graphics->SpawnControlWindow();
	
    Vector2f GOpos = m_enemy.GetTransform()->GetPosition();
    Vector2f Tpos = m_enemy.GetAI()->GetTargetPosition();
    m_enemy.GetAI()->SpawnControlWindow(GOpos, Tpos);

    m_tileMapEditor.SpawnControlWindow();
    m_player.SpawnControlWindow();
    m_imgui->EndRender();
#endif
    
    // Present Frame
	graphics->EndFrame();
}

void Level1::Update( const float dt )
{
    // Update entities
    UpdateTileMap( dt );
    m_player.Update( dt );
    m_enemy.Update( dt );
}

void Level1::UpdateTileMap(const float dt)
{
    static bool firstTimeTileMapDraw = true;
#if _DEBUG
    if (m_tileMapEditor.UpdateDrawOnceAvalible() || firstTimeTileMapDraw || m_tileMapEditor.UpdateDrawContinuousAvalible())
#else
    if (firstTimeTileMapDraw)
#endif
    {
        for (int i = 0; i < COLUMNS * ROWS; i++)
        {
            m_tileMapDraw[i].Update(dt);

            std::string texture = "Resources\\Textures\\";
            texture += m_tileMapEditor.GetTileTypeName(i);
            texture += ".png";

            m_tileMapDraw[i].GetSprite()->UpdateTex(graphics->GetDevice(), texture);
#if _DEBUG
            m_tileMapEditor.UpdateDrawOnceDone();
#endif
        }

        firstTimeTileMapDraw = false;
    }
}