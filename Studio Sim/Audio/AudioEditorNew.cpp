#include "stdafx.h"
#include "AudioEditorNew.h"

#include "Timer.h"
#include "FileLoading.h"
#include <algorithm>

#if _DEBUG
#include <imgui/imgui.h>
#endif

#define MAIN_SOUND_BANK_LIST "Resources\\Audio\\SoundBanksList.json"
#define SOUNDS_FILES_PATH  "Resources\\Audio\\"

AudioEditorNew::AudioEditorNew()
{
	LoadFromFileSoundBankLists();
	//LoadFromFileSoundBankFiles();
}

AudioEditorNew::~AudioEditorNew() { }

void AudioEditorNew::LoadFromFileSoundBankLists()
{
	// Load UI screens
	JsonLoading::LoadJson(SoundBanksList, MAIN_SOUND_BANK_LIST);
	//SortScreens();
}

//void AudioEditorNew::LoadFromFileSoundBankFiles()
//{
//	// Load screen widgets
//	for (unsigned int i = 0; i < SoundBanksList.size(); i++)
//	{
//		std::vector<JSONSoundFile> soundFileData;
//		JsonLoading::LoadJson(soundFileData, FOLDER_PATH_SOUND_FILES + SoundBanksList[i].file);
//		m_vSoundFileData.emplace(SoundBanksList[i].name, soundFileData);
//	}
//}

//void AudioEditorNew::SortScreens()
//{
//	// Sort screens by name for ImGui
//	std::vector<std::string> screenNames;
//	for (unsigned int i = 0; i < SoundBanksList.size(); i++)
//		screenNames.push_back(SoundBanksList[i].name);
//	sort(screenNames.begin(), screenNames.end());
//	std::vector<SoundBankList> tempScreenList;
//	for (unsigned int i = 0; i < screenNames.size(); i++)
//	{
//		for (unsigned int j = 0; j < SoundBanksList.size(); j++)
//		{
//			if (screenNames[i] == SoundBanksList[j].name)
//			{
//				tempScreenList.push_back(SoundBanksList[j]);
//			}
//		}
//	}
//	SoundBanksList = tempScreenList;
//}

#if _DEBUG
void AudioEditorNew::SaveToFileSoundBankLists()
{
	// Add check duplicate of name
	JsonLoading::SaveJson(SoundBanksList, MAIN_SOUND_BANK_LIST);
}

//void AudioEditorNew::SaveToFileSoundBankFiles()
//{
//	for (unsigned int i = 0; i < SoundBanksList.size(); i++)
//	{
//		for (std::map<std::string, std::vector<JSONSoundFile>>::iterator it = m_vSoundFileData.begin(); it != m_vSoundFileData.end(); it++)
//		{
//			if (SoundBanksList[i].name == it->first)
//			{
//				JsonLoading::SaveJson(it->second, FOLDER_PATH_SOUND_FILES + SoundBanksList[i].file);
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
			if (ImGui::BeginListBox("##Sound Banks List", ImVec2(-FLT_MIN, SoundBanksList.size() * ImGui::GetTextLineHeightWithSpacing() * 1.1f)))
			{
				int index = 0;
				for (auto const& [key, value] : SoundBanksList)
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
			if (ImGui::Button("Add New SoundBank"))
			{
				static int screenIdx = 0;
				std::string screenName = "Sound Bank " + std::to_string(screenIdx);
				SoundBanksList.push_back(SoundBankList({ screenName, SOUNDS_FILES_PATH + screenName + ".json" }));
				currentSoundBankFileIdx -= SoundBanksList.size() - 1;
			}
			ImGui::SameLine();
			if (ImGui::Button("Remove Sound Bank"))
			{
				if (SoundBanksList.size() > 1)
				{
					SoundBanksList.erase(SoundBanksList.begin() + currentSoundBankFileIdx);
					SoundBanksList.shrink_to_fit();
					currentSoundBankFileIdx -= 1;
				}
			}

			// Modify sound bank
			if (currentSoundBankFileIdx > -1)
			{
				// Update screen name
				static char buf[32] = "";
				static bool modifiedName = false;
				ImGui::Text("Screen Name: ");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 0.1f, 0.1f, 1.0f), SoundBanksList[currentSoundBankFileIdx].name.c_str());
				if (ImGui::InputText(std::string("##").append(SoundBanksList[currentSoundBankFileIdx].name).c_str(), buf, IM_ARRAYSIZE(buf)))
					modifiedName = true;
				if (modifiedName)
				{
					if (ImGui::Button(std::string("Save Name##").append(SoundBanksList[currentSoundBankFileIdx].name).c_str()))
					{
						// TODO: prevent user from setting duplicate names
						SoundBanksList[currentSoundBankFileIdx].name = buf;
						modifiedName = false;
					}
				}
				ImGui::NewLine();

				// Update screen widget file
				ImGui::Text("Screen Widget File: ");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 0.1f, 0.1f, 1.0f), SoundBanksList[currentSoundBankFileIdx].filePath.c_str());
				if (ImGui::Button("Load Widget File"))
				{
					if (FileLoading::OpenFileExplorer(m_sSelectedFile, m_sFilePath))
					{
						SoundBanksList[currentSoundBankFileIdx].filePath = m_sSelectedFile;
						std::string type = ".json";
						std::string::size_type idx = m_sSelectedFile.find(type);
						if (idx != std::string::npos)
							m_sSelectedFile.erase(idx, type.length());
						SoundBanksList[currentSoundBankFileIdx].name = m_sSelectedFile;

						//SortScreens();
						//LoadFromFileSoundBankFiles();
					}
				}
				ImGui::NewLine();


			}
		}
		ImGui::NewLine();

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
			//	std::string screenName = SoundBanksList[currentSoundBankFileIdx].name;
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