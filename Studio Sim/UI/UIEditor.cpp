#include "stdafx.h"
#include "UIEditor.h"
#include "Graphics.h"
#include "Timer.h"

#if _DEBUG
#include <imgui/imgui.h>
#endif

#define FOLDER_PATH "Resources\\UI\\"

UIEditor::UIEditor()
{
	LoadFromFile();
}

UIEditor::~UIEditor() { }

void UIEditor::LoadFromFile()
{
	JsonLoading::LoadJson( m_vUIScreenList, FOLDER_PATH + m_sJsonFile );
	for ( unsigned int i = 0; i < m_vUIScreenList.size(); i++ )
	{
		std::vector<UIScreenData> screenData;
		JsonLoading::LoadJson( screenData, FOLDER_PATH + m_vUIScreenList[i].file );
		m_vUIScreenData.emplace( m_vUIScreenList[i].name, screenData );
	}
}

#if _DEBUG
void UIEditor::SaveToFile_Screens()
{
	JsonLoading::SaveJson( m_vUIScreenList, FOLDER_PATH + m_sJsonFile );
}

void UIEditor::SaveToFile_Widgets()
{
	for ( unsigned int i = 0; i < m_vUIScreenList.size(); i++ )
	{
		for ( std::map<std::string, std::vector<UIScreenData>>::iterator it = m_vUIScreenData.begin(); it != m_vUIScreenData.end(); it++ )
		{
			if ( m_vUIScreenList[i].name == it->first )
			{
				JsonLoading::SaveJson( it->second, FOLDER_PATH + m_vUIScreenList[i].file );
			}
		}
	}
}

void UIEditor::SpawnControlWindow( const Graphics& gfx )
{
	static Timer timer;
	static float counter = 0.0f;
	static bool savedFile = false;

	if ( ImGui::Begin( "UI Editor", FALSE, ImGuiWindowFlags_AlwaysAutoResize ) )
	{
		// Save UI Screens
		if ( ImGui::Button( "Save screens?" ) )
		{
			SaveToFile_Widgets();
			savedFile = true;
		}
		ImGui::SameLine();

		// Save UI components
		if ( ImGui::Button( "Save widgets?" ) )
		{
			SaveToFile_Widgets();
			savedFile = true;
		}

		// Update save message
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

		// List of all UI screens currently defined
		static int currentIdx = -1;
		ImGui::Text( "UI Screen List" );
		if ( ImGui::BeginListBox( "##UI Screen List", ImVec2( -FLT_MIN, m_vUIScreenData.size() * ImGui::GetTextLineHeightWithSpacing() * 1.1f ) ) )
		{
			int index = 0;
			for ( auto const& [key, value] : m_vUIScreenData )
			{
				const bool isSelected = ( currentIdx == index );
				if ( ImGui::Selectable( key.c_str(), isSelected ) )
					currentIdx = index;

				if ( isSelected )
					ImGui::SetItemDefaultFocus();
				
				index++;
			}
			ImGui::EndListBox();
		}

		// Modifiy UI screens

		
		// Edit UI components for each screen
		int index = 0;
		for ( auto& [key, value] : m_vUIScreenData ) // loop each screen
		{
			if ( index == currentIdx )
			{
				for ( unsigned int i = 0; i < value.size(); i++ ) // loop ui elements on current screen
				{
					if ( ImGui::TreeNode( std::string( value[i].name ).append( " (" + value[i].type + ")" ).c_str() ) )
					{
						ImGui::NewLine();

						static char buf[32] = "";
						static bool modifiedName = false;
						ImGui::Text( "Widget Name: " );
						ImGui::SameLine();
						ImGui::TextColored( ImVec4( 1.0f, 0.1f, 0.1f, 1.0f ), value[i].name.c_str() );
						if ( ImGui::InputText( std::string( "##" ).append( value[i].name ).c_str(), buf, IM_ARRAYSIZE( buf ) ) )
							modifiedName = true;
						if ( modifiedName )
						{
							if ( ImGui::Button( std::string( "Save Name##" ).append( value[i].name ).c_str() ) )
							{
								// TODO: prevent user from setting duplicate names
								value[i].name = buf;
								modifiedName = false;
							}
						}
						ImGui::NewLine();

						ImGui::Text( "Widget Type: " );
						ImGui::SameLine();
						ImGui::TextColored( ImVec4( 1.0f, 0.1f, 0.1f, 1.0f ), value[i].type.c_str() );
						static int currentType = 0;
						const char* comboPreview = value[i].type.c_str();
						if ( ImGui::BeginCombo( std::string( "##" ).append( value[i].name ).append( value[i].type ).c_str(), comboPreview ) )
						{
							for ( unsigned int j = 0; j < m_vUITypes.size(); j++ )
							{
								const bool isSelected = ( currentType == j );
								if ( ImGui::Selectable( m_vUITypes[j].c_str(), isSelected ) )
								{
									currentType = j;
									value[i].type = m_vUITypes[j];
								}

								if ( isSelected )
									ImGui::SetItemDefaultFocus();
							}
							ImGui::EndCombo();
						}
						// TODO: the widget should change depending on what the user sets
						ImGui::NewLine();

						ImGui::Text( "Position" );
						float max = ( gfx.GetWidth() > gfx.GetHeight() ? gfx.GetWidth() : gfx.GetHeight() );
						static float position[2] = { value[i].position[0], value[i].position[1] };
						ImGui::SliderFloat2( std::string( "##Position" ).append( key ).append( value[i].name ).c_str(), position, 0.0f, max, "%.1f" );
						value[i].position = { position[0], position[1] };
						ImGui::NewLine();

						ImGui::Text( "Scale" );
						static float scale[2] = { value[i].scale[0], value[i].scale[1] };
						ImGui::SliderFloat2( std::string( "##Scale" ).append( key ).append( value[i].name ).c_str(), scale, 0.0f, max, "%.1f" );
						value[i].scale = { scale[0], scale[1] };
						ImGui::NewLine();

						ImGui::TreePop();
					}
				}
			}
			index++;
		}

		// Modify UI components

	}
	ImGui::End();
}
#endif