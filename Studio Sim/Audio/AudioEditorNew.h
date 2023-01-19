#pragma once
#ifndef AUDIOEDITORNEW_H
#define AUDIOEDITORNEW_H

#include "Audio/AudioEngine.h"

#include "JsonLoading.h"

struct SoundBankList // these are the lists to hold file paths to different sound bank jsons
{
	std::string name;
	std::string filePath;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SoundBankList, name, filePath)

//struct JSONSoundFile
//{
//	std::string name;
//	std::string type;
//	std::vector<float> position;
//	std::vector<float> scale;
//};
//NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(JSONSoundFile, name, type, position, scale)

class AudioEditorNew
{
public:
	AudioEditorNew();
	~AudioEditorNew();

	void LoadFromFileSoundBankLists();
	//void LoadFromFileSoundBankFiles();
	//void SortScreens();
#if _DEBUG
	void SaveToFileSoundBankLists();
	//void SaveToFileSoundBankFiles();
	void SpawnControlWindow();
#endif // _DEBUG
private:
	std::string m_sFilePath;
	std::string m_sFileContent;
	std::string m_sSelectedFile;

	std::vector<std::string> m_vSoundTypes = { "SFX", "MUSIC" };

	std::vector<SoundBankList> SoundBanksList; // list of UI screens
	std::map<std::string, std::vector<JSONSoundFile>> m_vSoundFileData; // list of UI components for a given screen
};

#endif // AUDIOEDITORNEW_H