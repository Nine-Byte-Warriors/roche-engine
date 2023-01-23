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
        if ( !m_renderWindow.Initialize( &m_input, hInstance, "Midnight Harvest -Engine-", "TutorialWindowClass", width, height ) )
		    return false;

        // Initialize m_graphics
        if ( !m_graphics.Initialize( m_renderWindow.GetHWND(), width, height ) )
		    return false;

        // Initialize audio
        AudioEngine::GetInstance()->Initialize(1.0f, 1.0f, 1.0f, 1, 4);

        // Initialize input
        m_input.Initialize( m_renderWindow );
#if _DEBUG
        m_imgui.Initialize( m_renderWindow.GetHWND(), m_graphics.GetDevice(), m_graphics.GetContext() );
#endif

        // Initialize levels
        m_pLevels.push_back( std::make_shared<Level>( "Farm", m_uCurrLevelId ) );
        m_pLevels.push_back( std::make_shared<Level>( "Shop", m_uCurrLevelId ) );
#if _DEBUG
        m_pLevels[0]->Initialize( &m_graphics, &m_uiManager, &m_imgui );
        m_pLevels[1]->Initialize( &m_graphics, &m_uiManager, &m_imgui );
#else
        m_pLevel[0]->Initialize( &m_graphics, &m_uiManager );
        m_pLevel[1]->Initialize( &m_graphics, &m_uiManager );
#endif

        m_uLevel_IDs.push_back( m_stateMachine.Add( m_pLevels[0] ) );
        m_uLevel_IDs.push_back( m_stateMachine.Add( m_pLevels[1] ) );
        m_stateMachine.SwitchTo( m_uLevel_IDs[0] );
        m_uCurrLevelId = 0;
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
    m_stateMachine.Render_Start();

    // Level Editor
    static bool shouldSwitchLevel = false;
    if ( ImGui::Begin( "Level Editor", FALSE, ImGuiWindowFlags_AlwaysAutoResize ) )
    {
        ImGui::Text( "UI Screen List" );
		if ( ImGui::BeginListBox( "##UI Screen List", ImVec2( -FLT_MIN, m_pLevels.size() * ImGui::GetTextLineHeightWithSpacing() * 1.1f ) ) )
		{
			int index = 0;
			for ( unsigned int i = 0; i < m_pLevels.size(); i++ )
			{
				const bool isSelected = ( m_uCurrLevelId == index );
				if ( ImGui::Selectable( m_pLevels[i]->GetLevelName().c_str(), isSelected ) )
                {
                    m_uCurrLevelId = index;
                    if ( m_uCurrLevelId == m_uLevel_IDs[index] )
                    {
                        shouldSwitchLevel = true;
                        break;
                    }
                }

				if ( isSelected )
					ImGui::SetItemDefaultFocus();
				
				index++;
			}
			ImGui::EndListBox();
		}

        if ( ImGui::Button( "Switch To Selected" ) && shouldSwitchLevel )
        {
            m_stateMachine.SwitchTo( m_uLevel_IDs[m_uCurrLevelId] );
            shouldSwitchLevel = false;
        }

        if ( ImGui::Button( "Add New Level" ) )
	    {
		    static int levelIdx = 0;
		    std::string levelName = "New Level " + std::to_string( levelIdx );
		    m_pLevels.push_back( std::make_shared<Level>( levelName, m_uCurrLevelId ) );
#if _DEBUG
            m_pLevels[m_uCurrLevelId]->Initialize( &m_graphics, &m_uiManager, &m_imgui );
#else
            m_pLevel[m_uCurrLevelId]->Initialize( &m_graphics, &m_uiManager );
#endif
            m_uLevel_IDs.push_back( m_stateMachine.Add( m_pLevels[m_uCurrLevelId] ) );
	    }
	    ImGui::SameLine();
	    if ( ImGui::Button( "Remove Current Level" ) )
	    {
		    if ( m_pLevels.size() > 1 )
		    {
                m_pLevels.erase( m_pLevels.begin() + m_uCurrLevelId );
                m_pLevels.shrink_to_fit();
                m_uLevel_IDs.erase( m_uLevel_IDs.begin() + m_uCurrLevelId );
                m_uLevel_IDs.shrink_to_fit();
                m_uCurrLevelId -= 1;
		    }
	    }
    }
    ImGui::End();

    m_stateMachine.Render_End();
}