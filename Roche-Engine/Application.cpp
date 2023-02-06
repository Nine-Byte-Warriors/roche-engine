#include "stdafx.h"
#include "Application.h"

#if _DEBUG
extern bool g_bDebug;
#include <imgui/imgui.h>
#endif

#define FOLDER_PATH "Resources\\"

bool Application::Initialize( HINSTANCE hInstance, int width, int height )
{
    try
    {
        // Initialize window
        if ( !m_renderWindow.Initialize( &m_input, hInstance, "Roche Engine", "TutorialWindowClass", width, height ) )
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
        JsonLoading::LoadJson( m_vLevelData, FOLDER_PATH + m_sJsonFile );

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
            std::shared_ptr<Level> level = std::make_shared<Level>( m_vLevelData[i].name );
#if _DEBUG
            level->Initialize( &m_graphics, &m_uiManager, &m_imgui );
            level->SetAudioJson( m_vLevelData[i].audio );
            AudioEngine::GetInstance()->LoadSoundBanksList(m_vLevelData[m_iActiveLevelIdx].audio);
#else
            level->Initialize( &m_graphics, &m_uiManager );
#endif
            level->SetEntityJson( m_vLevelData[i].entity );
            level->CreateTileMap();
            //level->SetTileMapJson( m_vLevelData[i].tmBack, m_vLevelData[i].tmFront );
            level->SetUIJson( m_vLevelData[i].ui );

            m_pLevels.push_back( std::move( level ) );
            //m_sLevelNames.push_back( m_stateMachine.Add( m_pLevels[i] ));
            m_stateMachine.Add(m_pLevels[i]);
        }
        m_stateMachine.SwitchTo( "Menu" );
        m_sCurrentLevelName = "Menu";
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

#if _DEBUG
    if ( g_bDebug )
    {
        // Level Editor
        static bool shouldSwitchLevel = false;
        if ( ImGui::Begin( "Level Editor", FALSE, ImGuiWindowFlags_AlwaysAutoResize ) )
        {
            static Timer timer;
            static float counter = 0.0f;
            static bool savedFile = false;
            if ( ImGui::Button( "Save Levels?" ) )
            {
                JsonLoading::SaveJson( m_vLevelData, FOLDER_PATH + m_sJsonFile );
                savedFile = true;
            }
            if ( savedFile )
		    {
			    ImGui::TextColored( ImVec4( 0.1f, 1.0f, 0.1f, 1.0f ), "FILE SAVED!" );
			    counter += timer.GetDeltaTime();
			    if ( counter > 3.0f )
			    {
				    counter = 0.0f;
				    savedFile = false;
			    }
		    }
            ImGui::NewLine();

            ImGui::Text( "Level List" );
		    if ( ImGui::BeginListBox( "##Level List", ImVec2( -FLT_MIN, m_pLevels.size() * ImGui::GetTextLineHeightWithSpacing() * 1.1f ) ) )
		    {
			    int index = 0;
			    for ( unsigned int i = 0; i < m_pLevels.size(); i++ )
			    {
				    const bool isSelected = ( m_sCurrentLevelName == m_pLevels[index]->GetLevelName() );
				    if ( ImGui::Selectable( m_pLevels[i]->GetLevelName().c_str(), isSelected ) )
                    {
                        m_sCurrentLevelName = m_pLevels[index]->GetLevelName();
                        if ( m_sCurrentLevelName == m_sLevelNames[index] )
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

            static char buf[32] = "";
            static bool modifiedName = false;
            if ( ImGui::InputText( std::string( "##" ).append( m_stateMachine.GetCurrentLevel()->GetLevelName().c_str() ).c_str(), buf, IM_ARRAYSIZE( buf ) ) )
			    modifiedName = true;
		    if ( modifiedName )
		    {
			    if ( ImGui::Button( std::string( "Save Name##" ).append( m_stateMachine.GetCurrentLevel()->GetLevelName().c_str() ).c_str() ) )
			    {
				    m_stateMachine.GetCurrentLevel()->SetLevelName( buf );
                    m_pLevels[m_iActiveLevelIdx]->SetLevelName( buf );
				    m_vLevelData[m_iActiveLevelIdx].name = buf;
				    modifiedName = false;
			    }
		    }
            ImGui::NewLine();

            // Handle level switching
            if ( ImGui::Button( "Switch To" ) && shouldSwitchLevel )
            {
                m_stateMachine.SwitchTo( m_sCurrentLevelName );
                shouldSwitchLevel = false;
            }
            ImGui::SameLine();

            // Add/remove level
            if ( ImGui::Button( "Add Level" ) )
	        {
		        static int levelIdx = 0;
		        std::string levelName = "New Level " + std::to_string( levelIdx );
		        m_pLevels.push_back( std::make_shared<Level>( levelName ) );
                m_pLevels[m_pLevels.size() - 1]->Initialize(&m_graphics, &m_uiManager, &m_imgui);
                m_sLevelNames.push_back( m_stateMachine.Add( m_pLevels[m_pLevels.size() - 1] ) );
	        }
            ImGui::SameLine();

	        if ( ImGui::Button( "Remove Level" ) )
	        {
		      //  if ( m_pLevels.size() > 1 )
		      //  {
        //            m_pLevels.erase( m_pLevels.begin() + m_iCurrLevelId );
        //            m_pLevels.shrink_to_fit();

        //            m_stateMachine.Remove( m_uLevel_IDs[m_iCurrLevelId] );
        //            m_uLevel_IDs.erase( m_uLevel_IDs.begin() + m_iCurrLevelId );
        //            m_uLevel_IDs.shrink_to_fit();

        //            m_iCurrLevelId -= 1;
				    //if ( m_iCurrLevelId < 0 )
					   // m_iCurrLevelId = 0;

        //            m_stateMachine.SwitchTo( m_uLevel_IDs[m_iCurrLevelId] );
		      //  }
	        }
            ImGui::NewLine();

            // Active level options
            if ( m_sCurrentLevelName != "" )
            {
                // Get current level info
                for( unsigned int i = 0; i < m_vLevelData.size(); i++ )
                    if ( m_stateMachine.GetCurrentLevel()->GetLevelName() == m_vLevelData[i].name )
                        m_iActiveLevelIdx = i;

                ImGui::Text( "Audio Manager" );
                ImGui::SameLine();
                ImGui::TextColored( ImVec4( 1.0f, 0.0f, 0.0f, 1.0f ), m_vLevelData[m_iActiveLevelIdx].audio.c_str() );
                if ( ImGui::Button( "Switch Audio Manager?" ) )
                {
                    if ( FileLoading::OpenFileExplorer( m_sAudioFile, m_sFilePath ) )
                    {
                        m_vLevelData[m_iActiveLevelIdx].audio = m_sAudioFile;
                        m_pLevels[m_iActiveLevelIdx]->SetAudioJson( m_sAudioFile );
                        AudioEngine::GetInstance()->UnloadAllAudio();
                        AudioEngine::GetInstance()->LoadSoundBanksList(m_sAudioFile);
                    }
                }
                ImGui::NewLine();

                ImGui::Text( "Entity Manager" );
                ImGui::SameLine();
                ImGui::TextColored( ImVec4( 1.0f, 0.0f, 0.0f, 1.0f ), m_vLevelData[m_iActiveLevelIdx].entity.c_str() );
                if ( ImGui::Button( "Switch Entity Manager?" ) )
                {
                    if ( FileLoading::OpenFileExplorer( m_sEntityFile, m_sFilePath ) )
                    {
                        m_vLevelData[m_iActiveLevelIdx].entity = m_sEntityFile;
                        m_pLevels[m_iActiveLevelIdx]->SetEntityJson( m_sEntityFile );
                        m_pLevels[m_iActiveLevelIdx]->CreateEntity();
                    }
                }
                ImGui::NewLine();

                ImGui::Text( "Tile Map Back" );
                ImGui::SameLine();
                ImGui::TextColored( ImVec4( 1.0f, 0.0f, 0.0f, 1.0f ), m_vLevelData[m_iActiveLevelIdx].tmBack.c_str() );
                if ( ImGui::Button( "Switch Tile Map Back" ) )
                {
                    if ( FileLoading::OpenFileExplorer( m_sTileMapBackFile, m_sFilePath ) )
                    {
                        m_vLevelData[m_iActiveLevelIdx].tmBack = m_sTileMapBackFile;
                        m_pLevels[m_iActiveLevelIdx]->SetTileMapJson( m_sTileMapBackFile, m_vLevelData[m_iActiveLevelIdx].tmFront );
                    }
                }
                ImGui::NewLine();

                ImGui::Text( "Tile Map Front" );
                ImGui::SameLine();
                ImGui::TextColored( ImVec4( 1.0f, 0.0f, 0.0f, 1.0f ), m_vLevelData[m_iActiveLevelIdx].tmFront.c_str() );
                if ( ImGui::Button( "Switch Tile Map Front?" ) )
                {
                    if ( FileLoading::OpenFileExplorer( m_sTileMapFrontFile, m_sFilePath ) )
                    {
                        m_vLevelData[m_iActiveLevelIdx].tmFront = m_sTileMapFrontFile;
                        m_pLevels[m_iActiveLevelIdx]->SetTileMapJson( m_vLevelData[m_iActiveLevelIdx].tmBack, m_sTileMapFrontFile );
                    }
                }
                ImGui::NewLine();

                ImGui::Text( "UI Manager" );
                ImGui::SameLine();
                ImGui::TextColored( ImVec4( 1.0f, 0.0f, 0.0f, 1.0f ), m_vLevelData[m_iActiveLevelIdx].ui.c_str() );
                if ( ImGui::Button( "Switch UI Manager?" ) )
                {
                    if ( FileLoading::OpenFileExplorer( m_sUIFile, m_sFilePath ) )
                    {
                        m_vLevelData[m_iActiveLevelIdx].ui = m_sUIFile;
                        m_pLevels[m_iActiveLevelIdx]->SetUIJson( m_sUIFile );
                        m_pLevels[m_iActiveLevelIdx]->CreateUI();
                    }
                }
            }
        }
        ImGui::End();
    }
#endif

    m_stateMachine.Render_End();
}