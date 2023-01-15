#include "stdafx.h"
#include "Application.h"

#if _DEBUG
#include <imgui/imgui.h>
#endif

bool Application::Initialize( HINSTANCE hInstance, int width, int height )
{
    try
    {
        // Initialize window
        if ( !m_renderWindow.Initialize( &m_input, hInstance, "DirectX 11 Studio Sim Project", "TutorialWindowClass", width, height ) )
		    return false;

        // Initialize m_graphics
        if ( !m_graphics.Initialize( m_renderWindow.GetHWND(), width, height ) )
		    return false;

        // Initialize audio
        AudioEngine::GetInstance()->Initialize(1.0f, 1.0f, 1.0f, 1, 2);
        AudioEngine::GetInstance()->PlayAudio(L"quietlaugh", SFX); // remove later, just test
        //AudioEngine::GetInstance()->PlayAudio(L"piano2", SFX); // remove later, just test

        // Initialize input
        m_input.Initialize( m_renderWindow );
#if _DEBUG
        m_imgui.Initialize( m_renderWindow.GetHWND(), m_graphics.GetDevice(), m_graphics.GetContext() );
#endif

        // Initialize levels
        m_pLevel1 = std::make_shared<Level1>();
#if _DEBUG
        m_pLevel1->Initialize( &m_graphics, &m_imgui );
#else
        m_pLevel1->Initialize( &m_graphics );
#endif

        m_uLevel1_ID = m_stateMachine.Add( m_pLevel1 );
        m_stateMachine.SwitchTo( m_uLevel1_ID );
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
#if _DEBUG
    // Useful for finding dx memory leaks
    ID3D11Debug* debugDevice = nullptr;
    m_graphics.GetDevice()->QueryInterface( __uuidof( ID3D11Debug ), reinterpret_cast<void**>( &debugDevice ) );
    debugDevice->ReportLiveDeviceObjects( D3D11_RLDO_DETAIL );
    if ( debugDevice ) debugDevice->Release();
#endif
}

bool Application::ProcessMessages() noexcept
{
    // Process messages sent to the window
	return m_renderWindow.ProcessMessages();
}

void Application::Update()
{
    if ( m_renderWindow.GetIsStopNextFrame() )
    {
        // Update delta time
        float dt = m_timer.GetDeltaTime(); // capped at 60 fps
        if ( dt == 0.0f ) return;

        // Update input
        m_input.Update( dt );

        // Update audio engine
        AudioEngine::GetInstance()->Update();

        // Update current level
	    EventSystem::Instance()->ProcessEvents();
	    m_stateMachine.Update( dt );
    }
    else
    {
        m_renderWindow.SetIsStopNextFrame( false );
    }
}

void Application::Render()
{
    // Render current level
    m_stateMachine.Render();
}