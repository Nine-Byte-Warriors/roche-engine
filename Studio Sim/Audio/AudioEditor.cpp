#include "stdafx.h"
#include "AudioEditor.h"

#include "Timer.h"
#include "FileLoading.h"
#include <algorithm>
//#include <string>

#if _DEBUG
#include <imgui/imgui.h>
#endif

#define SOUND_BANK_LISTS_PATH "Resources\\Audio\\Sound Banks\\!SoundBankList.json"
#define SOUND_BANK_PATH  "Resources\\Audio\\Sound Banks\\"
#define SOUND_FILES_PATH  "Resources\\Audio\\"

#if _DEBUG

AudioEditor::AudioEditor()
{
	//LoadSoundFileInfoFromJSON("Resources\\Audio\\Sound Banks\\soundFiles.json"); // test remove later
	m_bSoundBankToLoad = false;
	LoadFromFileSoundBankLists();
	//LoadFromFileSoundBankFiles();
}

AudioEditor::~AudioEditor() { }

void AudioEditor::LoadFromFileSoundBankLists()
{
	// Load UI screens
	JsonLoading::LoadJson(m_vSoundBanksList, SOUND_BANK_LISTS_PATH);
	SortScreens();
}

void AudioEditor::LoadSoundFileInfoFromJSON(std::string loadFilePath)
{
	// Load Json File from given file path
	JsonLoading::LoadJson(m_vSoundFileInfo, loadFilePath);
}

void AudioEditor::SaveSoundFileInfoToJSON(std::string fileName)
{
	JsonLoading::SaveJson(m_vSoundFileInfo, SOUND_BANK_PATH + fileName + ".json");
}

//void AudioEditor::LoadFromFileSoundBankFiles()
//{
//	// Load screen widgets
//	for (unsigned int i = 0; i < m_vSoundBanksList.size(); i++)
//	{
//		std::vector<JSONSoundFile> soundFileData;
//		JsonLoading::LoadJson(soundFileData, FOLDER_PATH_SOUND_FILES + m_vSoundBanksList[i].file);
//		m_vSoundFileData.emplace(m_vSoundBanksList[i].name, soundFileData);
//	}
//}



void AudioEditor::SortScreens()
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

void AudioEditor::SaveToFileSoundBankLists()
{
	// Add check duplicate of name
	JsonLoading::SaveJson(m_vSoundBanksList, SOUND_BANK_LISTS_PATH);
}

//void AudioEditor::SaveToFileSoundBankFiles()
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

void AudioEditor::SpawnControlWindow()
{
	static Timer timer;
	static float counterSaveSoundBankList = 0.0f;
	static bool savedFileSoundBankList = false;
	static float counterSaveSoundBank = 0.0f;
	static bool savedFileSoundBank = false;
	static int currentSoundBankFileIdx = -1;
	static int previousSoundBankFileIdx = 5;
	static bool soundBankToLoad = false;

	if (ImGui::Begin("Audio Editor", FALSE, ImGuiWindowFlags_AlwaysAutoResize))
	{
		// Save Sound Banks List
		if (ImGui::Button("Save Sound Banks"))
		{
			SaveToFileSoundBankLists();
			savedFileSoundBankList = true;
		}
		ImGui::SameLine();

		// Save Sound Bank
		//if (ImGui::Button("Save Sound Bank Files"))
		//{
		//	SaveToFileSoundBankFiles();
		//	savedFileSoundBankList = true;
		//}

		// Update save message
		if (savedFileSoundBankList)
		{
			ImGui::TextColored(ImVec4(0.1f, 1.0f, 0.1f, 1.0f), "FILE SAVED!");
			counterSaveSoundBankList += timer.GetDeltaTime();
			if (counterSaveSoundBankList > 3.0f)
			{
				counterSaveSoundBankList = 0.0f;
				savedFileSoundBankList = false;
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
					if (ImGui::Selectable(key.c_str(), isSelected)) {
						previousSoundBankFileIdx = currentSoundBankFileIdx;
						currentSoundBankFileIdx = index;
						soundBankToLoad = true;
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();

					index++;
				}
				ImGui::EndListBox();
			}


			// Add/remove sound banks
			// TO DO: Check whether duplicate name exists, if so add up index and check again + ADD JSON when creating it, give option to rename it
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
				if (m_vSoundBanksList.size() > 1 && currentSoundBankFileIdx > -1)
				{
					m_vSoundBanksList.erase(m_vSoundBanksList.begin() + currentSoundBankFileIdx);
					m_vSoundBanksList.shrink_to_fit();
					currentSoundBankFileIdx -= 1;
				}
			}
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 0.1f, 0.1f, 1.0f), "DO NOT TOUCH");

			// Modify sound bank
			if (currentSoundBankFileIdx > -1)
			{
				if (soundBankToLoad) {
					// Unload audio from audio engine
					if (previousSoundBankFileIdx > -1) {
						for (unsigned int i = 0; i < m_vSoundFileInfo.size(); i++) {
							AudioEngine::GetInstance()->UnloadAudio(AudioEngine::GetInstance()->GetFileName(m_vSoundFileInfo[i].filePath), (AudioType)m_vSoundFileInfo[i].audioType);
						}
					}
					LoadSoundFileInfoFromJSON(m_vSoundBanksList[currentSoundBankFileIdx].filePath);
					// Load audio to audio engine
					AudioEngine::GetInstance()->LoadAudioFromJSON(m_vSoundBanksList[currentSoundBankFileIdx].filePath);
					soundBankToLoad = false;
				}

				if (ImGui::CollapsingHeader("Sound Bank Edit", ImGuiTreeNodeFlags_DefaultOpen)) {
					if (ImGui::CollapsingHeader("SFX List", ImGuiTreeNodeFlags_DefaultOpen)) {
						for (unsigned int i = 0; i < m_vSoundFileInfo.size(); i++) {
							if (m_vSoundFileInfo[i].audioType == SFX) {
								if (ImGui::TreeNode(m_vSoundFileInfo[i].filePath.c_str())) { 
									float defaultVolume = m_vSoundFileInfo[i].volume;
									if (ImGui::SliderFloat(std::string("Default Volume##").append(std::to_string(i)).append("default volume").c_str(), &defaultVolume, 0.0f, 1.0f)) {
										m_vSoundFileInfo[i].volume = defaultVolume;
										AudioEngine::GetInstance()->SetDefaultVolume(AudioEngine::GetInstance()->FindSoundBankFile(AudioEngine::GetInstance()->GetFileName(m_vSoundFileInfo[i].filePath), SFX), defaultVolume);
									}

									if (ImGui::Button(std::string("Play##").append(std::to_string(i)).append("play").c_str())) {
										AudioEngine::GetInstance()->PlayAudio(AudioEngine::GetInstance()->GetFileName(m_vSoundFileInfo[i].filePath), SFX);
									}
									ImGui::SameLine();

									if (ImGui::Button(std::string("Delete##").append(std::to_string(i)).append("delete").c_str())) {
										AudioEngine::GetInstance()->UnloadAudio(AudioEngine::GetInstance()->GetFileName(m_vSoundFileInfo[i].filePath), (AudioType)m_vSoundFileInfo[i].audioType);
										m_vSoundFileInfo.erase(m_vSoundFileInfo.begin() + i);
									}
									ImGui::TreePop();
								}
								ImGui::NewLine();
							}
						}
					}

					if (ImGui::Button(std::string("Add SFX##Add Sound to SFX Sound Bank").c_str())) {
						if (FileLoading::OpenFileExplorer(m_sSelectedFile, m_sFilePath))
						{
							JSONSoundFile newSound;
							newSound.filePath = SOUND_FILES_PATH + m_sSelectedFile;
							newSound.volume = 1.0f;
							newSound.audioType = SFX;
							m_vSoundFileInfo.emplace_back(newSound);
							AudioEngine::GetInstance()->LoadAudio(AudioEngine::GetInstance()->ConvertStringToWstring(newSound.filePath), newSound.volume, (AudioType)newSound.audioType);
						}
					}


					if (ImGui::CollapsingHeader("Music List", ImGuiTreeNodeFlags_DefaultOpen)) {
						for (unsigned int i = 0; i < m_vSoundFileInfo.size(); i++) {
							if (m_vSoundFileInfo[i].audioType == MUSIC) {
								if (ImGui::TreeNode(m_vSoundFileInfo[i].filePath.c_str())) { 
									float defaultVolume = m_vSoundFileInfo[i].volume;
									if (ImGui::SliderFloat(std::string("Default Volume##").append(std::to_string(i)).append("default volume").c_str(), &defaultVolume, 0.0f, 1.0f)) {
										m_vSoundFileInfo[i].volume = defaultVolume;
										AudioEngine::GetInstance()->SetDefaultVolume(AudioEngine::GetInstance()->FindSoundBankFile(AudioEngine::GetInstance()->GetFileName(m_vSoundFileInfo[i].filePath), MUSIC), defaultVolume);
									}

									if (ImGui::Button(std::string("Play##").append(std::to_string(i)).append("play").c_str())) {
										AudioEngine::GetInstance()->PlayAudio(AudioEngine::GetInstance()->GetFileName(m_vSoundFileInfo[i].filePath), MUSIC);
									}
									ImGui::SameLine();
									if (ImGui::Button(std::string("Unpause##").append(std::to_string(i)).append("Unpause").c_str())) {
										AudioEngine::GetInstance()->UnpauseMusic();
									}
									ImGui::SameLine();
									if (ImGui::Button(std::string("Pause##").append(std::to_string(i)).append("pause").c_str())) {
										AudioEngine::GetInstance()->PauseMusic();
									}
									ImGui::SameLine();
									if (ImGui::Button(std::string("Stop##").append(std::to_string(i)).append("stop").c_str())) {
										AudioEngine::GetInstance()->StopMusic();
									}
									ImGui::SameLine();

									if (ImGui::Button(std::string("Delete##").append(std::to_string(i)).append("delete").c_str())) {
										AudioEngine::GetInstance()->UnloadAudio(AudioEngine::GetInstance()->GetFileName(m_vSoundFileInfo[i].filePath), (AudioType)m_vSoundFileInfo[i].audioType);
										m_vSoundFileInfo.erase(m_vSoundFileInfo.begin() + i);
									}
									ImGui::TreePop();
								}
								ImGui::NewLine();

							}
						}
					}

					if (ImGui::Button(std::string("Add Music##Add Sound to Music Sound Bank").c_str())) {
						if (FileLoading::OpenFileExplorer(m_sSelectedFile, m_sFilePath))
						{
							JSONSoundFile newSound;
							newSound.filePath = SOUND_FILES_PATH + m_sSelectedFile;
							newSound.volume = 1.0f;
							newSound.audioType = MUSIC;
							m_vSoundFileInfo.emplace_back(newSound);
							AudioEngine::GetInstance()->LoadAudio(AudioEngine::GetInstance()->ConvertStringToWstring(newSound.filePath), newSound.volume, (AudioType)newSound.audioType);
						}
					}

					ImGui::NewLine();

					if (ImGui::Button(std::string("Save Sound Bank").c_str())) {
						SaveSoundFileInfoToJSON(m_vSoundBanksList[currentSoundBankFileIdx].name);
						savedFileSoundBank = true;
					}

					ImGui::SameLine();

					// Update save message
					if (savedFileSoundBank)
					{
						ImGui::TextColored(ImVec4(0.1f, 1.0f, 0.1f, 1.0f), "FILE SAVED!");
						counterSaveSoundBank += timer.GetDeltaTime();
						if (counterSaveSoundBank > 3.0f)
						{
							counterSaveSoundBank = 0.0f;
							savedFileSoundBank = false;
						}
					}


				}


			}
		}
		ImGui::NewLine();

	}
	ImGui::End();
}
#endif