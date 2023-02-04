#include "stdafx.h"
#include "UIEditor.h"

#include "Timer.h"
#include "Graphics.h"
#include "FileLoading.h"
#include "FileHandler.h"
#include <algorithm>

#if _DEBUG
#include <imgui/imgui.h>
#endif

#define FOLDER_PATH "Resources\\UI\\"
#define FOLDER_PATH_SCREENS "Resources\\UI\\Screens\\"

UIEditor::UIEditor()
{
	LoadFromFile_Screens();
	LoadFromFile_Widgets();
}

UIEditor::~UIEditor() { }

void UIEditor::SetJsonFile( const std::string& name )
{
	m_sJsonFile = name;
	LoadFromFile_Screens();
	LoadFromFile_Widgets();
}

void UIEditor::LoadFromFile_Screens()
{
	// Load UI screens
	m_vUIScreenData.clear();
	JsonLoading::LoadJson( m_vUIScreenData, FOLDER_PATH + m_sJsonFile );
	SortScreens();
	CreateScreens();
}

void UIEditor::SortScreens()
{
	// Sort screens by name for ImGui
	std::vector<std::string> screenNames;
	for ( unsigned int i = 0; i < m_vUIScreenData.size(); i++ )
		screenNames.push_back( m_vUIScreenData[i].name );
	sort( screenNames.begin(), screenNames.end() );
	std::vector<UIScreenData> tempScreenList;
	for ( unsigned int i = 0; i < screenNames.size(); i++ )
	{
		for ( unsigned int j = 0; j < m_vUIScreenData.size(); j++ )
		{
			if ( screenNames[i] == m_vUIScreenData[j].name )
			{
				tempScreenList.push_back( m_vUIScreenData[j] );
			}
		}
	}
	m_vUIScreenData = tempScreenList;
}

void UIEditor::CreateScreens()
{
	// Create screen objects
	if ( m_vUIScreens.size() > 0 )
		m_vUIScreens.clear();
	for ( unsigned int i = 0; i < m_vUIScreenData.size(); i++ )
	{
		std::shared_ptr<UIScreen> screen = std::make_shared<UIScreen>();
		m_vUIScreens.push_back( std::move( screen ) );
	}
}

void UIEditor::LoadFromFile_Widgets()
{
	// Load screen widgets
	if ( m_vUIWidgetData.size() > 0 )
		m_vUIWidgetData.clear();
	for ( unsigned int i = 0; i < m_vUIScreenData.size(); i++ )
	{
		std::vector<UIWidgetData> screenData;
		JsonLoading::LoadJson( screenData, FOLDER_PATH_SCREENS + m_vUIScreenData[i].file );
		m_vUIWidgetData.emplace( m_vUIScreenData[i].name, screenData );
	}

	CreateWidgets();
}

void UIEditor::CreateWidgets()
{
	// Create widget objects
	int index = 0;
	if ( m_vUIWidgets.size() > 0 )
		m_vUIWidgets.clear();
	for ( std::map<std::string, std::vector<UIWidgetData>>::iterator it = m_vUIWidgetData.begin(); it != m_vUIWidgetData.end(); it++ ) // widget struct
	{
		m_vUIWidgets.push_back( {} );
		for ( unsigned int j = 0; j < it->second.size(); j++ ) // widget struct
		{
			m_vUIWidgets[index].push_back( std::make_shared<Widget>( it->second[j].hide, it->second[j].zindex,
				it->second[j].name, it->second[j].type, it->second[j].action,
				XMFLOAT2( it->second[j].position[0], it->second[j].position[1] ),
				XMFLOAT2( it->second[j].scale[0], it->second[j].scale[1] ) ) );
		}
		index++;
	}
}

#if _DEBUG
void UIEditor::SaveToFile_Screens()
{
	JsonLoading::SaveJson( m_vUIScreenData, FOLDER_PATH + m_sJsonFile );
}

void UIEditor::SaveToFile_Widgets()
{
	for ( unsigned int i = 0; i < m_vUIScreenData.size(); i++ )
	{
		for ( std::map<std::string, std::vector<UIWidgetData>>::iterator it = m_vUIWidgetData.begin(); it != m_vUIWidgetData.end(); it++ )
		{
			if ( m_vUIScreenData[i].name == it->first )
			{
				JsonLoading::SaveJson( it->second, FOLDER_PATH_SCREENS + m_vUIScreenData[i].file );
			}
		}
	}
}

void UIEditor::Update( const float dt )
{
	int index = 0;
	for ( auto& [key, value] : m_vUIWidgetData ) // each ui screen
	{
		for ( unsigned int i = 0; i < value.size(); i++ ) // loop ui elements on current screen
		{
			m_vUIWidgets[index][i]->SetIsHidden( value[i].hide );
			m_vUIWidgets[index][i]->SetZIndex( value[i].zindex );
			m_vUIWidgets[index][i]->SetName( value[i].name );
			m_vUIWidgets[index][i]->SetType( value[i].type );
			m_vUIWidgets[index][i]->SetAction( value[i].action );
			m_vUIWidgets[index][i]->SetSize( { value[i].scale[0], value[i].scale[1] } );
			m_vUIWidgets[index][i]->SetPosition( { value[i].position[0], value[i].position[1] } );
		}
		index++;
	}
}

void UIEditor::SpawnControlWindow( const Graphics& gfx )
{
	static Timer timer;
	static float counter = 0.0f;
	static bool savedFile = false;
	static ImVec4 highlightCol = ImVec4( 1.0f, 0.1f, 0.1f, 1.0f );

	if ( ImGui::Begin( "UI Editor", FALSE, ImGuiWindowFlags_AlwaysAutoResize ) )
	{
		// Save UI Screens
		if ( ImGui::Button( "Save screens?" ) )
		{
			SaveToFile_Screens();
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

		static int widgetIdx = 0;
		if ( ImGui::CollapsingHeader( "Screens", ImGuiTreeNodeFlags_DefaultOpen ) )
		{
			// Show all screens at once?
			ImGui::NewLine();
			ImGui::Checkbox( "Show all screens?", &m_bShouldShowAll );
			ImGui::SameLine();
			ImGui::Checkbox( "Hide all screens?", &m_bShouldHideAll );
			ImGui::NewLine();

			// List of all UI screens currently defined
			ImGui::Text( "UI Screen List" );
			if ( ImGui::BeginListBox( "##UI Screen List", ImVec2( -FLT_MIN, m_vUIScreenData.size() * ImGui::GetTextLineHeightWithSpacing() * 1.1f ) ) )
			{
				int index = 0;
				for ( auto const& [key, value] : m_vUIScreenData )
				{
					const bool isSelected = ( m_iCurrentScreenIdx == index );
					if ( ImGui::Selectable( key.c_str(), isSelected ) )
						m_iCurrentScreenIdx = index;

					if ( isSelected )
						ImGui::SetItemDefaultFocus();

					index++;
				}
				ImGui::EndListBox();
			}
			ImGui::NewLine();

			// Modify screens
			if ( m_iCurrentScreenIdx > -1 )
			{
				// Update screen name
				static char buf[32] = "";
				static bool modifiedName = false;
				ImGui::Text( "Screen Name: " );
				ImGui::SameLine();
				ImGui::TextColored( highlightCol, m_vUIScreenData[m_iCurrentScreenIdx].name.c_str() );
				if ( ImGui::InputText( std::string( "##" ).append( m_vUIScreenData[m_iCurrentScreenIdx].name ).c_str(), buf, IM_ARRAYSIZE( buf ) ) )
					modifiedName = true;
				if ( modifiedName )
				{
					if ( ImGui::Button( std::string( "Save Name##" ).append( m_vUIScreenData[m_iCurrentScreenIdx].name ).c_str() ) )
					{
						// TODO: prevent user from setting duplicate names
						m_vUIScreenData[m_iCurrentScreenIdx].name = buf;
						modifiedName = false;
					}
				}
				ImGui::NewLine();

				// Update screen widget file
				ImGui::Text( "Screen Widget File: " );
				ImGui::SameLine();
				ImGui::TextColored( highlightCol, m_vUIScreenData[m_iCurrentScreenIdx].file.c_str() );
				if ( ImGui::Button( "Load Widget File" ) )
				{
					std::shared_ptr<FileHandler::FileObject>foLoad = FileHandler::FileDialog(foLoad)
						->UseOpenDialog()
						->ShowDialog()
						->StoreDialogResult();

					if (foLoad->HasPath())
					{
						m_vUIScreenData[m_iCurrentScreenIdx].file = foLoad->GetFilePath();
						m_vUIScreenData[m_iCurrentScreenIdx].name = foLoad->m_sFile;

						SortScreens();
						m_vUIScreens.clear();
						for ( unsigned int i = 0; i < m_vUIScreenData.size(); i++ )
						{
							std::shared_ptr<UIScreen> screen = std::make_shared<UIScreen>();
							m_vUIScreens.push_back( std::move( screen ) );
						}
						LoadFromFile_Widgets();
						m_bRequiresUpdate = true;
					}
				}
				ImGui::NewLine();

				// Add/remove screens
				if ( ImGui::Button( "Add New Screen" ) )
				{
					static int screenIdx = 0;
					std::string screenName = "Blank Screen " + std::to_string( screenIdx );
					m_vUIScreenData.push_back( UIScreenData( { screenName, std::string( screenName ).append( ".json" ) } ) );
					SortScreens();

					std::vector<UIWidgetData> widgetData;
					widgetData.push_back( UIWidgetData( { false, 0, "Blank Widget " + std::to_string( widgetIdx ), "Image", "", { 0.0f, 0.0f }, { 64.0f, 64.0f } } ) );
					m_vUIWidgetData.emplace( screenName, widgetData );
					widgetIdx++;

					m_iCurrentScreenIdx = m_vUIScreenData.size() - 1;
				}
				ImGui::SameLine();
				if ( ImGui::Button( "Remove Current Screen" ) )
				{
					if ( m_vUIScreenData.size() > 1 )
					{
						m_vUIScreenData.erase( m_vUIScreenData.begin() + m_iCurrentScreenIdx );
						m_vUIScreenData.shrink_to_fit();
						m_iCurrentScreenIdx -= 1;
					}
				}
			}
		}
		ImGui::NewLine();

		if ( m_iCurrentScreenIdx > -1 )
		{
			if ( ImGui::CollapsingHeader( "Widgets", ImGuiTreeNodeFlags_DefaultOpen ) )
			{
				// Edit UI components for each screen
				int index = -1;
				for ( auto& [key, value] : m_vUIWidgetData ) // loop each screens data struct
				{
					index++;
					if ( index != m_iCurrentScreenIdx )
						continue;

					for ( unsigned int i = 0; i < value.size(); i++ ) // loop ui elements on current screen
					{
						ImGui::PushID( i );
						if ( ImGui::TreeNode( std::string( value[i].name )
							.append( " (" + value[i].type + ")" )
							.append( " (" + std::to_string( value[i].zindex ) + ")" ).c_str() ) )
						{
							ImGui::NewLine();

							// TODO: prevent user from setting duplicate names
							const int bufSize = 32;
							static char buf[bufSize] = "";
							static bool modifiedName = false;
							ImGui::Text( "Widget Name: " );
							ImGui::SameLine();
							ImGui::TextColored( highlightCol, value[i].name.c_str() );
							if ( ImGui::InputText( std::string( "##WidgetName" ).append( value[i].name ).append( value[i].type ).append( std::to_string( i ) ).c_str(), buf, IM_ARRAYSIZE( buf ) ) )
								modifiedName = true;
							if ( modifiedName )
							{
								if ( ImGui::Button( std::string( "Save Name##" ).append( value[i].name ).c_str() ) )
								{
									value[i].name = buf;
									memset( buf, 0, bufSize );
									modifiedName = false;
								}
							}
							ImGui::NewLine();

							ImGui::Text( "Widget Type: " );
							ImGui::SameLine();
							ImGui::TextColored( highlightCol, value[i].type.c_str() );
							static int currentType = 0;
							const char* comboPreview = value[i].type.c_str();
							if ( ImGui::BeginCombo( std::string( "##WidgetType" ).append( value[i].name ).append( std::to_string( i ) ).c_str(), comboPreview ) )
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
							ImGui::NewLine();

							if ( value[i].action.size() > 0 )
							{
								ImGui::Text( "Action: " );
								ImGui::SameLine();
								ImGui::TextColored( highlightCol, value[i].action.c_str() );
								ImGui::NewLine();
							}

							ImGui::Text( "Position" );
							float max = ( gfx.GetWidth() > gfx.GetHeight() ? gfx.GetWidth() : gfx.GetHeight() );
							float position[2] = { value[i].position[0], value[i].position[1] };
							ImGui::DragFloat2( std::string( "##Position" ).append( key ).append( value[i].name ).c_str(), position, 1.0f, -max, max * 2.0f, "%.1f" );
							value[i].position = { position[0], position[1] };
							ImGui::NewLine();

							ImGui::Text( "Scale" );
							float scale[2] = { value[i].scale[0], value[i].scale[1] };
							ImGui::DragFloat2( std::string( "##Scale" ).append( key ).append( value[i].name ).c_str(), scale, 1.0f, 0.0f, max, "%.1f" );
							value[i].scale = { scale[0], scale[1] };
							ImGui::NewLine();

							ImGui::Text( "Z Index" );
							int prevZIndex = value[i].zindex;
							if ( ImGui::InputInt( std::string( "##Z Index" ).append( key ).append( value[i].name ).c_str(), &value[i].zindex ) )
							{
								// Prevent z index from going out of bounds
								if ( value[i].zindex > value.size() - 1 )
									value[i].zindex = value.size() - 1;

								if ( value[i].zindex < 0 )
									value[i].zindex = 0;

								// Update z values of all other widgets
								for ( unsigned int j = 0; j < value.size(); j++ )
								{
									if ( j != i && value[j].zindex == value[i].zindex )
									{
										int tempZIndex = value[j].zindex;
										value[j].zindex = prevZIndex;
										value[i].zindex = tempZIndex;
										break;
									}
								}
							}
							ImGui::NewLine();

							ImGui::Text( "Hide Widget?" );
							ImGui::SameLine();
							ImGui::Checkbox( std::string( "##Hide Widget?" ).append( key ).append( value[i].name ).c_str(), &value[i].hide );
							ImGui::NewLine();

							// Remove the current widget?
							if ( ImGui::Button( "Remove Widget" ) )
							{
								value.erase( value.begin() + i );
								value.shrink_to_fit();
								m_bRequiresUpdate = true;
								CreateWidgets();
							}
							ImGui::NewLine();

							ImGui::TreePop();
						}
						ImGui::PopID();
					}
				}

				// Add a new widget
				if ( ImGui::Button( "Add New Widget" ) )
				{
					std::string screenName = m_vUIScreenData[m_iCurrentScreenIdx].name;
					for ( auto& [key, value] : m_vUIWidgetData )
					{
						if ( key == screenName )
						{
							value.push_back( UIWidgetData( false, value.size(),
								"Blank Widget " + std::to_string( widgetIdx ),
								"Image", "", { 0.0f, 0.0f }, { 64.0f, 64.0f } ) );
							m_bRequiresUpdate = true;
							CreateWidgets();
							widgetIdx++;
							break;
						}
					}
				}
			}
		}
	}
	ImGui::End();
}
#endif