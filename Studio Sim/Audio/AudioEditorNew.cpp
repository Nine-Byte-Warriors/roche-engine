#include "stdafx.h"
#include "AudioEditorNew.h"

#include "Timer.h"
#include "FileLoading.h"
#include <algorithm>

#if _DEBUG
#include <imgui/imgui.h>
#endif

#define SOUND_BANK_LISTS_PATH "Resources\\Audio\\Sound Banks\\!SoundBankList.json"
#define SOUND_BANK_PATH  "Resources\\Audio\\Sound Banks\\"
#define SOUND_FILES_PATH  "Resources\\Audio\\"

AudioEditorNew::AudioEditorNew()
{
	LoadSoundFileInfoFromJSON("Resources\\Audio\\Sound Banks\\soundFiles.json"); // test remove later
	m_bSoundBankToLoad = false;
	LoadFromFileSoundBankLists();
	//LoadFromFileSoundBankFiles();
}

AudioEditorNew::~AudioEditorNew() { }

void AudioEditorNew::LoadFromFileSoundBankLists()
{
	// Load UI screens
	JsonLoading::LoadJson(m_vSoundBanksList , SOUND_BANK_LISTS_PATH);
	SortScreens();
}

void AudioEditorNew::LoadSoundFileInfoFromJSON(std::string loadFilePath)
{
	// Load Json File from given file path
	JsonLoading::LoadJson(m_vSoundFileInfo, loadFilePath);
}

void AudioEditorNew::SaveSoundFileInfoToJSON(std::string fileName)
{
	JsonLoading::SaveJson(m_vSoundFileInfo, SOUND_BANK_PATH + fileName + ".json");
}

//void AudioEditorNew::LoadFromFileSoundBankFiles()
//{
//	// Load screen widgets
//	for (unsigned int i = 0; i < m_vSoundBanksList.size(); i++)
//	{
//		std::vector<JSONSoundFile> soundFileData;
//		JsonLoading::LoadJson(soundFileData, FOLDER_PATH_SOUND_FILES + m_vSoundBanksList[i].file);
//		m_vSoundFileData.emplace(m_vSoundBanksList[i].name, soundFileData);
//	}
//}


//void AudioEditorNew::SortScreens()
//{
//	// Sort screens by name for ImGui
//	std::vector<std::string> screenNames;
//	for (unsigned int i = 0; i < m_vSoundBanksList.size(); i++)
//		screenNames.push_back(m_vSoundBanksList[i].name);
//	sort(screenNames.begin(), screenNames.end());
//	std::vector<SoundBanksList> tempScreenList;
//	for (unsigned int i = 0; i < screenNames.size(); i++)
//	{
//		for (unsigned int j = 0; j < m_vSoundBanksList.size(); j++)
//		{
//			if (screenNames[i] == m_vSoundBanksList[j].name)
//			{
//				tempScreenList.push_back(m_vSoundBanksList[j]);
//			}
//		}
//	}
//	m_vSoundBanksList = tempScreenList;
//}

void AudioEditorNew::SortScreens()
{
	// Sort screens by name for ImGui
	std::vector<std::string> screenNames;
	for (unsigned int i = 0; i < m_vSoundBanksList.size(); i++)
		screenNames.push_back(m_vSoundBanksList[i].name);
	sort(screenNames.begin(), screenNames.end());
	std::vector<SoundBanksList> tempScreenList;
	for (unsigned int i = 0; i < screenNames.size(); i++)
	{
		for (unsigned int j = 0; j < m_vSoundBanksList.size(); j++)
		{
			if (screenNames[i] == m_vSoundBanksList[j].name)
			{
				tempScreenList.push_back(m_vSoundBanksList[j]);
			}
		}
	}
	m_vSoundBanksList = tempScreenList;
}

#if _DEBUG
void AudioEditorNew::SaveToFileSoundBankLists()
{
	// Add check duplicate of name
	JsonLoading::SaveJson(m_vSoundBanksList, SOUND_BANK_LISTS_PATH);
}

//void AudioEditorNew::SaveToFileSoundBankFiles()
//{
//	for (unsigned int i = 0; i < m_vSoundBanksList.size(); i++)
//	{
//		for (std::map<std::string, std::vector<JSONSoundFile>>::iterator it = m_vSoundFileData.begin(); it != m_vSoundFileData.end(); it++)
//		{
//			if (m_vSoundBanksList[i].name == it->first)
//			{
//				JsonLoading::SaveJson(it->second, FOLDER_PATH_SOUND_FILES + m_vSoundBanksList[i].file);
//			}
//		}
//	}
//}

void AudioEditorNew::SpawnControlWindow()
{
	static Timer timer;
	static float counter = 0.0f;
	static bool savedFile = false;
	static int currentSoundBankFileIdx = -1;

	if (ImGui::Begin("Audio Editor", FALSE, ImGuiWindowFlags_AlwaysAutoResize))
	{
		// Save Sound Banks List
		if (ImGui::Button("Save Sound Banks"))
		{
			SaveToFileSoundBankLists();
			savedFile = true;
		}
		ImGui::SameLine();

		// Save Sound Bank
		//if (ImGui::Button("Save Sound Bank Files"))
		//{
		//	SaveToFileSoundBankFiles();
		//	savedFile = true;
		//}

		// Update save message
		if (savedFile)
		{
			ImGui::TextColored(ImVec4(0.1f, 1.0f, 0.1f, 1.0f), "FILE SAVED!");
			counter += timer.GetDeltaTime();
			if (counter > 3.0f)
			{
				counter = 0.0f;
				savedFile = false;
			}
		}
		ImGui::NewLine();

		if (ImGui::CollapsingHeader("Sound Banks", ImGuiTreeNodeFlags_DefaultOpen))
		{
			// List of all UI screens currently defined
			ImGui::Text("Sound Banks List");
			if (ImGui::BeginListBox("##Sound Banks List", ImVec2(-FLT_MIN, m_vSoundBanksList.size() * ImGui::GetTextLineHeightWithSpacing() * 1.1f)))
			{
				int index = 0;
				for (auto const& [key, value] : m_vSoundBanksList)
				{
					const bool isSelected = (currentSoundBankFileIdx == index);
					if (ImGui::Selectable(key.c_str(), isSelected))
						currentSoundBankFileIdx = index;

					if (isSelected)
						ImGui::SetItemDefaultFocus();

					index++;
				}
				ImGui::EndListBox();
			}

			// Add/remove sound banks
			// TO DO: Check whether duplicate name exists, if so add up index and check again
			if (ImGui::Button("Add New Sound Bank"))
			{
				static int screenIdx = 0;
				std::string screenName = "Sound Bank " + std::to_string(screenIdx);
				m_vSoundBanksList.push_back(SoundBanksList({ screenName, SOUND_BANK_PATH + screenName + ".json" }));
				currentSoundBankFileIdx -= m_vSoundBanksList.size() - 1;
			}
			ImGui::SameLine();
			if (ImGui::Button("Remove Sound Bank"))
			{
				if (m_vSoundBanksList.size() > 1)
				{
					m_vSoundBanksList.erase(m_vSoundBanksList.begin() + currentSoundBankFileIdx);
					m_vSoundBanksList.shrink_to_fit();
					currentSoundBankFileIdx -= 1;
				}
			}

			// Modify sound bank
			if (currentSoundBankFileIdx > -1)
			{
				if (ImGui::Button("Manage Sound Bank"))
				{
					m_bSoundBankToLoad = true;
					// Load all files of the Sound Bank JSON
				}
				//// Update screen name
				//static char buf[32] = "";
				//static bool modifiedName = false;
				//ImGui::Text("Screen Name: ");
				//ImGui::SameLine();
				//ImGui::TextColored(ImVec4(1.0f, 0.1f, 0.1f, 1.0f), m_vSoundBanksList[currentSoundBankFileIdx].name.c_str());
				//if (ImGui::InputText(std::string("##").append(m_vSoundBanksList[currentSoundBankFileIdx].name).c_str(), buf, IM_ARRAYSIZE(buf)))
				//	modifiedName = true;
				//if (modifiedName)
				//{
				//	if (ImGui::Button(std::string("Save Name##").append(m_vSoundBanksList[currentSoundBankFileIdx].name).c_str()))
				//	{
				//		// TODO: prevent user from setting duplicate names
				//		m_vSoundBanksList[currentSoundBankFileIdx].name = buf;
				//		modifiedName = false;
				//	}
				//}
				//ImGui::NewLine();

				//// Update screen widget file
				//ImGui::Text("Screen Widget File: ");
				//ImGui::SameLine();
				//ImGui::TextColored(ImVec4(1.0f, 0.1f, 0.1f, 1.0f), m_vSoundBanksList[currentSoundBankFileIdx].filePath.c_str());
				//if (ImGui::Button("Load Widget File"))
				//{
				//	if (FileLoading::OpenFileExplorer(m_sSelectedFile, m_sFilePath))
				//	{
				//		m_vSoundBanksList[currentSoundBankFileIdx].filePath = m_sSelectedFile;
				//		std::string type = ".json";
				//		std::string::size_type idx = m_sSelectedFile.find(type);
				//		if (idx != std::string::npos)
				//			m_sSelectedFile.erase(idx, type.length());
				//		m_vSoundBanksList[currentSoundBankFileIdx].name = m_sSelectedFile;

				//		SortScreens();
				//		// LOAD chosen sound bank
				//	}
				//}
				//ImGui::NewLine();


			}
		}
		ImGui::NewLine();

		ImGui::Text("Active Sound Bank: "); // TODO: add name of active sound bank

		if (ImGui::CollapsingHeader("Sound Bank Edit", ImGuiTreeNodeFlags_DefaultOpen)) {
			int index = 0;
			if (m_bSoundBankToLoad == true) {
					if (ImGui::CollapsingHeader("SFX List", ImGuiTreeNodeFlags_DefaultOpen)) {
						for (unsigned int i = 0; i < m_vSoundFileInfo.size(); i++) {
							if (m_vSoundFileInfo[i].audioType == SFX) {
								if (ImGui::TreeNode("Sfx")) { // add their own labels bs
									ImGui::Text("TestText");
									// populate it with sfx stuff in treenode
									ImGui::TreePop();
								}
								ImGui::NewLine();
							}
						}
					}

					if (ImGui::CollapsingHeader("Music List", ImGuiTreeNodeFlags_DefaultOpen)) {
						for (unsigned int i = 0; i < m_vSoundFileInfo.size(); i++) {
							if (m_vSoundFileInfo[i].audioType == MUSIC) {
								if (ImGui::TreeNode("Music")) {
									ImGui::Text("TestText");
									// populate it with sfx stuff in treenode
									ImGui::TreePop();
								}
								ImGui::NewLine();

							}
						}
					}
				/*m_bSoundBankToLoad = false;*/
			}


		}
		
		//if (ImGui::CollapsingHeader("Widgets", ImGuiTreeNodeFlags_DefaultOpen))
		//{

		//	// Edit UI components for each screen
		//	int index = 0;
		//	for (auto& [key, value] : m_vSoundFileData) // loop each screens data struct
		//	{
		//		if (index == currentSoundBankFileIdx)
		//		{
		//			for (unsigned int i = 0; i < value.size(); i++) // loop sound bank file elements on current screen
		//			{
		//				if (ImGui::TreeNode(std::string(value.at(i).filePath).c_str())) // add audio type?
		//				{
		//					ImGui::NewLine();

		//					//// TODO: prevent user from setting duplicate names
		//					//const int bufSize = 32;
		//					//static char buf[bufSize] = "";
		//					//static bool modifiedName = false;
		//					//ImGui::Text("Widget Name: ");
		//					//ImGui::SameLine();
		//					//ImGui::TextColored(ImVec4(1.0f, 0.1f, 0.1f, 1.0f), value[i].filePath.c_str());
		//					//if (ImGui::InputText(std::string("##").append(value[i].filePath).c_str(), buf, IM_ARRAYSIZE(buf)))
		//					//	modifiedName = true;
		//					//if (modifiedName)
		//					//{
		//					//	if (ImGui::Button(std::string("Save Name##").append(value[i].filePath).c_str()))
		//					//	{
		//					//		value[i].filePath = buf;
		//					//		memset(buf, 0, bufSize);
		//					//		modifiedName = false;
		//					//	}
		//					//}
		//					//ImGui::NewLine();

		//					//ImGui::Text("Widget Type: ");
		//					//ImGui::SameLine();
		//					//ImGui::TextColored(ImVec4(1.0f, 0.1f, 0.1f, 1.0f), value[i].type.c_str());
		//					//static int currentType = 0;
		//					//const char* comboPreview = value[i].type.c_str();
		//					//if (ImGui::BeginCombo(std::string("##").append(value[i].name).append(value[i].type).c_str(), comboPreview))
		//					//{
		//					//	for (unsigned int j = 0; j < m_vSoundTypes.size(); j++)
		//					//	{
		//					//		const bool isSelected = (currentType == j);
		//					//		if (ImGui::Selectable(m_vSoundTypes[j].c_str(), isSelected))
		//					//		{
		//					//			currentType = j;
		//					//			value[i].type = m_vSoundTypes[j];
		//					//		}

		//					//		if (isSelected)
		//					//			ImGui::SetItemDefaultFocus();
		//					//	}
		//					//	ImGui::EndCombo();
		//					//}
		//					//// TODO: the widget should change depending on what the user sets
		//					//ImGui::NewLine();

		//					//ImGui::Text("Position");
		//					//float max = (gfx.GetWidth() > gfx.GetHeight() ? gfx.GetWidth() : gfx.GetHeight());
		//					//static float position[2] = { value[i].position[0], value[i].position[1] };
		//					//ImGui::SliderFloat2(std::string("##Position").append(key).append(value[i].name).c_str(), position, 0.0f, max, "%.1f");
		//					//value[i].position = { position[0], position[1] };
		//					//ImGui::NewLine();

		//					//ImGui::Text("Scale");
		//					//static float scale[2] = { value[i].scale[0], value[i].scale[1] };
		//					//ImGui::SliderFloat2(std::string("##Scale").append(key).append(value[i].name).c_str(), scale, 0.0f, max, "%.1f");
		//					//value[i].scale = { scale[0], scale[1] };
		//					ImGui::NewLine();

		//					// Remove the current widget?
		//					if (ImGui::Button("Remove Widget"))
		//					{
		//						value.erase(value.begin() + i);
		//						value.shrink_to_fit();
		//					}
		//					ImGui::NewLine();

		//					ImGui::TreePop();
		//				}
		//			}
		//		}
		//		index++;
		//	}

			// Add a new widget
			//if (ImGui::Button("Add New Widget"))
			//{
			//	std::string screenName = m_vSoundBanksList[currentSoundBankFileIdx].name;
			//	for (auto& [key, value] : m_vSoundFileData)
			//	{
			//		if (key == screenName)
			//		{
			//			static int widgetIdx = 0;
			//			value.push_back(JSONSoundFile("Blank Widget " + std::to_string(widgetIdx), "Image", { 0.0f, 0.0f }, { 64.0f, 64.0f }));
			//		}
			//	}
			//}
		//}
	}
	ImGui::End();
}
#endif