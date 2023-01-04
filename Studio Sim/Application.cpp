#include "stdafx.h"
#include "Application.h"
#include <imgui/imgui.h>

bool Application::Initialize( HINSTANCE hInstance, int width, int height )
{
    try
    {
        // Initialize window
        if ( !renderWindow.Initialize( &m_input, hInstance, "DirectX 11 Studio Sim Project", "TutorialWindowClass", width, height ) )
		    return false;

        // Initialize graphics
        if ( !graphics.Initialize( renderWindow.GetHWND(), width, height ) )
		    return false;

        // Initialize input
        m_camera.Initialize( XMFLOAT3( 0.0f, 0.0f, -3.0f ), width, height );
        m_input.Initialize( renderWindow, m_camera );
        m_imgui.Initialize( renderWindow.GetHWND(), graphics.GetDevice(), graphics.GetContext() );

        // Initialize constant buffers
        HRESULT hr = m_cbMatrices.Initialize( graphics.GetDevice(), graphics.GetContext() );
	    COM_ERROR_IF_FAILED( hr, "Failed to create 'Matrices' constant buffer!" );
        
        // Initialize game objects
	    hr = m_cube.InitializeMesh( graphics.GetDevice(), graphics.GetContext() );
        COM_ERROR_IF_FAILED(hr, "Failed to create 'cube' object!");

        // Initialize systems
        m_spriteFont = std::make_unique<SpriteFont>( graphics.GetDevice(), L"Resources\\Fonts\\open_sans_ms_16_bold.spritefont" );
        m_spriteBatch = std::make_unique<SpriteBatch>( graphics.GetContext() );
        m_postProcessing.Initialize( graphics.GetDevice() );
    }
    catch ( COMException& exception )
	{
		ErrorLogger::Log( exception );
		return false;
	}

    return true;
}

void Application::CleanupDevice()
{
#ifdef _DEBUG
    // Useful for finding dx memory leaks
    ID3D11Debug* debugDevice = nullptr;
    graphics.GetDevice()->QueryInterface( __uuidof( ID3D11Debug ), reinterpret_cast<void**>( &debugDevice ) );
    debugDevice->ReportLiveDeviceObjects( D3D11_RLDO_DETAIL );
    if ( debugDevice ) debugDevice->Release();
#endif
}

bool Application::ProcessMessages() noexcept
{
    // Process messages sent to the window
	return renderWindow.ProcessMessages();
}

void Application::Update()
{
    // Update delta time
    float dt = m_timer.GetDeltaTime(); // capped at 60 fps
    if ( dt == 0.0f ) return;

    // Update input
    m_input.Update( dt );
    if ( windowResized )
    {
        m_imgui.Initialize( renderWindow.GetHWND(), graphics.GetDevice(), graphics.GetContext() );
        m_camera.SetProjectionValues( 75.0f,
            static_cast<float>( graphics.GetWidth() ) /
            static_cast<float>( graphics.GetHeight() ),
            0.01f, 100.0f );
    }

    // Update the cube transform, material etc. 
    m_cube.Update( dt );
}

void Application::Render()
{
#pragma RENDER_PASSES
    // Standard pass
    graphics.BeginFrame();

    // Render objects
    graphics.UpdateRenderState();
    m_cube.UpdateBuffers( m_cbMatrices, m_camera );
    graphics.GetContext()->VSSetConstantBuffers( 0u, 1u, m_cbMatrices.GetAddressOf() );
    m_cube.Draw( graphics.GetContext() );
#pragma endregion

#pragma region POST_PROCESSING
    // Render text
    m_spriteBatch->Begin();
    static XMFLOAT2 textPosition = { graphics.GetWidth() * 0.5f, graphics.GetHeight() * 0.96f };
    std::function<XMFLOAT2( const wchar_t* )> DrawOutline = [&]( const wchar_t* text ) mutable -> XMFLOAT2
    {
        XMFLOAT2 originF = XMFLOAT2( 1.0f, 1.0f );
        XMVECTOR origin = m_spriteFont->MeasureString( text ) / 2.0f;
        XMStoreFloat2( &originF, origin );

        // Draw outline
        m_spriteFont->DrawString( m_spriteBatch.get(), text,
            XMFLOAT2( textPosition.x + 1.0f, textPosition.y + 1.0f ), Colors::Black, 0.0f, originF );
        m_spriteFont->DrawString( m_spriteBatch.get(), text,
            XMFLOAT2( textPosition.x - 1.0f, textPosition.y + 1.0f ), Colors::Black, 0.0f, originF );
        m_spriteFont->DrawString( m_spriteBatch.get(), text,
            XMFLOAT2( textPosition.x - 1.0f, textPosition.y - 1.0f ), Colors::Black, 0.0f, originF );
        m_spriteFont->DrawString( m_spriteBatch.get(), text,
            XMFLOAT2( textPosition.x + 1.0f, textPosition.y - 1.0f ), Colors::Black, 0.0f, originF );

        return originF;
    };
    const wchar_t* text = L"This is example text.";
    XMFLOAT2 originF = DrawOutline( text );
    m_spriteFont->DrawString( m_spriteBatch.get(), text, textPosition,
        Colors::Green, 0.0f, originF, XMFLOAT2( 1.0f, 1.0f ) );
    m_spriteBatch->End();

    // Render scene to texture
    graphics.BeginRTT();
    m_postProcessing.Bind( graphics.GetContext(), graphics.GetRenderTarget() );

    // Render imgui windows
    m_imgui.BeginRender();
    m_imgui.SpawnInstructionWindow();
    m_postProcessing.SpawnControlWindow();
    m_cube.SpawnControlWindow();
    m_imgui.EndRender();

    // Present frame
    graphics.EndFrame();
#pragma endregion
}