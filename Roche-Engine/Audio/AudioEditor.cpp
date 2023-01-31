#include "stdafx.h"
#include "AudioEditor.h"

#include "Timer.h"
#include <FileHandler.h>
#include <algorithm>
//#include <string>

#if _DEBUG
#include <imgui/imgui.h>
#endif

#define SOUND_BANK_LISTS_PATH "Resources\\Audio\\Sound Banks\\"
#define SOUND_BANK_PATH  "Resources\\Audio\\Sound Banks\\"
#define SOUND_FILES_PATH  "Resources\\Audio\\"

#if _DEBUG
AudioEditor::AudioEditor()
{
	//LoadSoundFileInfoFromJSON("Resources\\Audio\\Sound Banks\\soundFiles.json"); // test remove later
	m_bSoundBankToLoad = false;
	m_sActiveSoundBankList = "None";
	m_sActiveSoundBankPath = "None";
	//LoadFromFileSoundBankLists();
	//LoadFromFileSoundBankFiles();
}

AudioEditor::~AudioEditor() { }

void AudioEditor::SetJsonFile(const std::string& name)
{
	m_sSoundBankFile = name;
	m_sActiveSoundBankList = name;
	JsonLoading::LoadJson(m_vSoundBanksList, SOUND_BANK_LISTS_PATH + m_sSoundBankFile);
	SortScreens();
}

void AudioEditor::SpawnSoundBankWindow(AudioType audioType)
{
	std::string audioTypeString;

	switch (audioType)
	{
	case SFX:
		audioTypeString = "SFX";
		break;
	case MUSIC:
		audioTypeString = "Music";
		break;
	default:
		audioTypeString = "Unknown Type";
		break;
	}
	if (ImGui::CollapsingHeader((audioTypeString + " List").c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
		for (unsigned int i = 0; i < m_vSoundFileInfo.size(); i++) {
			if (m_vSoundFileInfo[i].audioType == audioType) {
				if (ImGui::TreeNode(m_vSoundFileInfo[i].filePath.c_str())) {
					float defaultVolume = m_vSoundFileInfo[i].volume;
					bool randomPitchEnabled = m_vSoundFileInfo[i].randomPitch;
					float pitchMinValue = m_vSoundFileInfo[i].pitchMin;
					float pitchMaxValue = m_vSoundFileInfo[i].pitchMax;


					if (ImGui::SliderFloat(std::string("Default Volume##").append(std::to_string(i)).append("default volume").c_str(), &defaultVolume, 0.0f, 1.0f)) {
						m_vSoundFileInfo[i].volume = defaultVolume;
						AudioEngine::GetInstance()->SetDefaultVolume(AudioEngine::GetInstance()->FindSoundBankFile(AudioEngine::GetInstance()->GetFileName(m_vSoundFileInfo[i].filePath), audioType), defaultVolume);
					}

					if (audioType == SFX) {
						if (ImGui::Checkbox(std::string("Random Pitch##").append(std::to_string(i)).append("random pitch").c_str(), &randomPitchEnabled)) {
							m_vSoundFileInfo[i].randomPitch = !m_vSoundFileInfo[i].randomPitch;
							AudioEngine::GetInstance()->SetRandomPitch(AudioEngine::GetInstance()->FindSoundBankFile(AudioEngine::GetInstance()->GetFileName(m_vSoundFileInfo[i].filePath), audioType));

						}

						if (m_vSoundFileInfo[i].randomPitch) {
							if (ImGui::SliderFloat(std::string("Minimum Pitch##").append(std::to_string(i)).append("minimum pitch").c_str(), &pitchMinValue, 0.5f, 1.5f)) {
								m_vSoundFileInfo[i].pitchMin = pitchMinValue;
								AudioEngine::GetInstance()->SetPitchMin(AudioEngine::GetInstance()->FindSoundBankFile(AudioEngine::GetInstance()->GetFileName(m_vSoundFileInfo[i].filePath), audioType), pitchMinValue);
							}

							if (ImGui::SliderFloat(std::string("Maximum Pitch##").append(std::to_string(i)).append("maximum pitch").c_str(), &pitchMaxValue, 0.5f, 1.5f)) {
								m_vSoundFileInfo[i].pitchMax = pitchMaxValue;
								AudioEngine::GetInstance()->SetPitchMax(AudioEngine::GetInstance()->FindSoundBankFile(AudioEngine::GetInstance()->GetFileName(m_vSoundFileInfo[i].filePath), audioType), pitchMaxValue);
							}
						}
					}

					if (ImGui::Button(std::string("Play##").append(std::to_string(i)).append("play").c_str())) {
						AudioEngine::GetInstance()->PlayAudio(AudioEngine::GetInstance()->GetFileName(m_vSoundFileInfo[i].filePath), audioType);
					}

					ImGui::SameLine();
					if (audioType == MUSIC) { // If its MUSIC, then spawn these additional buttons
						if (ImGui::Button(std::string("Unpause##").append(std::to_string(i)).append("unpause").c_str())) {
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
					}
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

	if (ImGui::Button(("Add to " + audioTypeString + " list" + "##" + audioTypeString).c_str())) {
		// Call the FileDialog Builder and store the result in a file object.
		std::shared_ptr<FileHandler::FileObject>foLoad = FileHandler::FileDialog(foLoad)
			->UseOpenDialog()	// Choose the dialog to use.
			->ShowDialog()		// Show the dialog.
			->StoreDialogResult();	// Store the result.

		if (foLoad->HasPath())
		{
			bool duplicateFound = false;
			JSONSoundFile newSound;

			if (foLoad->m_sExt == "wav") {
				if (foLoad->GetFullPath().find(SOUND_FILES_PATH) != std::string::npos) {
					newSound.name = foLoad->m_sFile;
					newSound.filePath =  SOUND_FILES_PATH + foLoad->GetFilePath();
					newSound.volume = 1.0f;
					newSound.randomPitch = false;
					newSound.pitchMin = 1.0f;
					newSound.pitchMax = 1.0f;
					newSound.audioType = audioType;

					for (unsigned int i = 0; i < m_vSoundFileInfo.size(); i++) {
						if (m_vSoundFileInfo[i].name == newSound.name) {
							duplicateFound = true;
						}
					}

					if (!duplicateFound) {
						m_vSoundFileInfo.emplace_back(newSound);
						AudioEngine::GetInstance()->LoadAudio(StringHelper::StringToWide(newSound.filePath), newSound.volume, (AudioType)newSound.audioType,
																	newSound.randomPitch, newSound.pitchMin, newSound.pitchMax);
					}
				}
				else {
					ErrorLogger::Log("AudioEditor::SpawnSoundBankWindow: Invalid file path");
				}
			}
			else {
				ErrorLogger::Log("AudioEditor::SpawnSoundBankWindow: Invalid file format chosen to load into the Sound Bank");
			}
		}
	}
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
	//JsonLoading::SaveJson(m_vSoundBanksList, SOUND_BANK_LISTS_PATH + std::string("!SoundBankList_New.json"));

	std::shared_ptr<FileHandler::FileObject> foSave;

	foSave = FileHandler::FileDialog(foSave)	// pass file object to the FileDialog Buidler.
		->UseSaveDialog()	// Choose the dialog to use.
		->ShowDialog()		// Show the dialog.
		->StoreDialogResult();	// Store the result.

	// Check if the file object has a file path/name.
	if (foSave->HasPath()) {
		m_sActiveSoundBankList = foSave->GetFilePath();
		JsonLoading::SaveJson(m_vSoundBanksList, foSave->GetJsonPath()); // Save the file.
	}
}

void AudioEditor::LoadFromFileSoundBankLists()
{
	// Call the FileDialog Builder and store the result in a file object.
	std::shared_ptr<FileHandler::FileObject>foLoad = FileHandler::FileDialog(foLoad)
		->UseOpenDialog()	// Choose the dialog to use.
		->ShowDialog()		// Show the dialog.
		->StoreDialogResult();	// Store the result.

	if (foLoad->HasPath()) {
		if (foLoad->GetFullPath().find(SOUND_BANK_LISTS_PATH) != std::string::npos) {
			// Load UI screens
			JsonLoading::LoadJson(m_vSoundBanksList, SOUND_BANK_LISTS_PATH + foLoad->GetFilePath());
			if (m_vSoundBanksList.size() == 0)
			{
				return;
			}
			m_sActiveSoundBankList = foLoad->GetFilePath();
			SortScreens();
		}
	}
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
	static float counterSaveSoundBank = 0.0f;
	static bool savedFileSoundBank = false;
	static int currentSoundBankFileIdx = -1;
	static int previousSoundBankFileIdx = 5;
	static bool soundBankToLoad = false;

	if (ImGui::Begin("Audio Editor", FALSE, ImGuiWindowFlags_AlwaysAutoResize))
	{
		// Save Sound Banks List
		if (ImGui::Button("Save Sound Bank List"))
		{
			SaveToFileSoundBankLists();
		}
		ImGui::SameLine();

		if (ImGui::Button("Load Sound Bank List"))
		{
			LoadFromFileSoundBankLists();
		}

		ImGui::NewLine();

		ImGui::Text(std::string("Active Sound Bank List:").c_str());
		ImGui::Text(m_sActiveSoundBankList.c_str());
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
						m_sActiveSoundBankPath = m_vSoundBanksList.at(currentSoundBankFileIdx).filePath;
						soundBankToLoad = true;
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();

					index++;
				}
				ImGui::EndListBox();
			}

			ImGui::Text(std::string("Active Sound Bank file path:").c_str());
			ImGui::Text(m_sActiveSoundBankPath.c_str());
			ImGui::NewLine();

			// Add/remove sound banks
			// TO DO: Check whether duplicate name exists, if so add up index and check again + ADD JSON when creating it, give option to rename it
			if (ImGui::Button("Add New Sound Bank"))
			{

				std::shared_ptr<FileHandler::FileObject> foSave;

				foSave = FileHandler::FileDialog(foSave)	// pass file object to the FileDialog Buidler.
					->UseSaveDialog()	// Choose the dialog to use.
					->ShowDialog()		// Show the dialog.
					->StoreDialogResult();	// Store the result.

				// Check if the file object has a file path/name.
				if (foSave->HasPath()) {
					std::vector<JSONSoundFile> soundFileInfo;
					JsonLoading::SaveJson(soundFileInfo, foSave->GetJsonPath()); // Save the file.
					m_vSoundBanksList.push_back(SoundBanksList({ foSave->m_sFile, SOUND_BANK_PATH + foSave->GetFilePath()}));
					currentSoundBankFileIdx -= m_vSoundBanksList.size() - 1;
				}
			}
			ImGui::SameLine();

			if (ImGui::Button("Load Existing Sound Bank"))
			{
				bool duplicateFound = false;

				// Call the FileDialog Builder and store the result in a file object.
				std::shared_ptr<FileHandler::FileObject>foLoad = FileHandler::FileDialog(foLoad)
					->UseOpenDialog()	// Choose the dialog to use.
					->ShowDialog()		// Show the dialog.
					->StoreDialogResult();	// Store the result.

				if (foLoad->HasPath()) {
					for (int i = 0; m_vSoundBanksList.size() > i; i++) {
						if (m_vSoundBanksList.at(i).name == foLoad->m_sFile) {
							duplicateFound = true;
						}
					}

					if (!duplicateFound) {
						std::vector<JSONSoundFile> soundFileInfo;
						JsonLoading::LoadJson(soundFileInfo, foLoad->GetJsonPath());
						if (soundFileInfo.size() != 0)
						{
							m_vSoundBanksList.push_back(SoundBanksList({ foLoad->m_sFile, SOUND_BANK_PATH + foLoad->GetFilePath() }));
							currentSoundBankFileIdx -= m_vSoundBanksList.size() - 1;
						}
					}

				}
			}
			ImGui::NewLine();

			if (ImGui::Button("Remove Sound Bank"))
			{
				if (m_vSoundBanksList.size() > 1 && currentSoundBankFileIdx > -1)
				{
					m_vSoundBanksList.erase(m_vSoundBanksList.begin() + currentSoundBankFileIdx);
					m_vSoundBanksList.shrink_to_fit();
					currentSoundBankFileIdx -= 1;
				}
			}
			ImGui::NewLine();

			// Modify sound bank
			if (currentSoundBankFileIdx > -1)
			{
				if (soundBankToLoad) {
					// Unload audio from audio engine
					AudioEngine::GetInstance()->StopAllAudio();
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

					SpawnSoundBankWindow(SFX);
					SpawnSoundBankWindow(MUSIC);

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