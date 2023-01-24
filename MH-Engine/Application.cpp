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

        // Load level data
		std::string folderPath = "Resources/";
        JsonLoading::LoadJson( m_vLevelData, folderPath + m_sJsonFile );

        // Sort levels by name for ImGui
	    std::vector<std::string> levelNames;
	    for ( unsigned int i = 0; i < m_vLevelData.size(); i++ )
		    levelNames.push_back( m_vLevelData[i].name );
	    sort( levelNames.begin(), levelNames.end() );
	    std::vector<LevelData> tempLevelList;
	    for ( unsigned int i = 0; i < levelNames.size(); i++ )
	    {
		    for ( unsigned int j = 0; j < m_vLevelData.size(); j++ )
		    {
			    if ( levelNames[i] == m_vLevelData[j].name )
			    {
                    tempLevelList.push_back( m_vLevelData[j] );
			    }
		    }
	    }
        m_vLevelData = tempLevelList;

        // Create levels
        for ( unsigned int i = 0; i < m_vLevelData.size(); i++ )
        {
            std::shared_ptr<Level> level = std::make_shared<Level>( m_vLevelData[i].name, m_iCurrLevelId );
#if _DEBUG
            level->Initialize( &m_graphics, &m_uiManager, &m_imgui );
#else
            level->Initialize( &m_graphics, &m_uiManager );
#endif
            m_pLevels.push_back( std::move( level ) );
            m_uLevel_IDs.push_back( m_stateMachine.Add( m_pLevels[i] ) );
        }
        m_stateMachine.SwitchTo( m_uLevel_IDs[0] );
        m_iCurrLevelId = 0;
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
        ImGui::Text( "Level List" );
		if ( ImGui::BeginListBox( "##Level List", ImVec2( -FLT_MIN, m_pLevels.size() * ImGui::GetTextLineHeightWithSpacing() * 1.1f ) ) )
		{
			int index = 0;
			for ( unsigned int i = 0; i < m_pLevels.size(); i++ )
			{
				const bool isSelected = ( m_iCurrLevelId == index );
				if ( ImGui::Selectable( m_pLevels[i]->GetLevelName().c_str(), isSelected ) )
                {
                    m_iCurrLevelId = index;
                    if ( m_iCurrLevelId == m_uLevel_IDs[index] )
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

        ImGui::NewLine();
        ImGui::Text( "Active Level: " );
        ImGui::SameLine();
        ImGui::TextColored( ImVec4( 1.0f, 0.0f, 0.0f, 1.0f ), m_stateMachine.GetCurrentLevel()->GetLevelName().c_str() );
        ImGui::NewLine();

        // Handle level switching
        if ( ImGui::Button( "Switch To" ) && shouldSwitchLevel )
        {
            m_stateMachine.SwitchTo( m_uLevel_IDs[m_iCurrLevelId] );
            shouldSwitchLevel = false;
        }

        // Add/remove level
        if ( ImGui::Button( "Add Level" ) )
	    {
		    static int levelIdx = 0;
		    std::string levelName = "New Level " + std::to_string( levelIdx );
		    m_pLevels.push_back( std::make_shared<Level>( levelName, m_iCurrLevelId ) );
#if _DEBUG
            m_pLevels[m_iCurrLevelId]->Initialize( &m_graphics, &m_uiManager, &m_imgui );
#else
            m_pLevel[m_iCurrLevelId]->Initialize( &m_graphics, &m_uiManager );
#endif
            m_uLevel_IDs.push_back( m_stateMachine.Add( m_pLevels[m_iCurrLevelId] ) );
	    }

	    if ( ImGui::Button( "Remove Level" ) )
	    {
		    if ( m_pLevels.size() > 1 )
		    {
                m_pLevels.erase( m_pLevels.begin() + m_iCurrLevelId );
                m_pLevels.shrink_to_fit();

                m_stateMachine.Remove( m_uLevel_IDs[m_iCurrLevelId] );
                m_uLevel_IDs.erase( m_uLevel_IDs.begin() + m_iCurrLevelId );
                m_uLevel_IDs.shrink_to_fit();

                m_iCurrLevelId -= 1;
				if ( m_iCurrLevelId < 0 )
					m_iCurrLevelId = 0;

                m_stateMachine.SwitchTo( m_uLevel_IDs[m_iCurrLevelId] );
		    }
	    }

        // Active level options
        if ( m_iCurrLevelId > -1 )
        {
            //ImGui::Text(  );
            //if ( ImGui::Button( "UI Manager" ) )
            //{
            //    if ( FileLoading::OpenFileExplorer( m_sUIFile, m_sFilePath ) )
            //    {
            //
            //    }
            //}
        }
    }
    ImGui::End();

    m_stateMachine.Render_End();
}