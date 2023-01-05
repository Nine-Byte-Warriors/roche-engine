#include "stdafx.h"
#include "Application.h"
#include <imgui/imgui.h>
#include <thread>

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

        // Initialize levels        
        level1 = std::make_shared<Level1>( stateMachine );
        std::thread first( &Level1::Initialize, level1, &graphics, &m_camera, &m_imgui );
        first.join();

		level1_ID = stateMachine.Add( level1 );
        stateMachine.SwitchTo( level1_ID );
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

    // Update current level
	stateMachine.Update( dt );
	EventSystem::Instance()->ProcessEvents();
}

void Application::Render()
{
    // Render current level
    stateMachine.Render();
}