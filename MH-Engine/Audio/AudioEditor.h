#pragma once
#ifndef AUDIOEDITOR_H
#define AUDIOEDITOR_H

#include "Audio/AudioEngine.h"

#include "JsonLoading.h"

struct SoundBanksList // these are the lists to hold file paths to different sound bank jsons
{
	std::string name;
	std::string filePath;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SoundBanksList, name, filePath)

class AudioEditor
{
public:
#if _DEBUG
	AudioEditor();
	~AudioEditor();
	void SortScreens();
	void SpawnControlWindow();
	void SaveToFileSoundBankLists();
	void LoadFromFileSoundBankLists();
	//void SaveToFileSoundBankFiles();
	
	// sound bank handle
	void LoadSoundFileInfoFromJSON(std::string loadFilePath);
	void SaveSoundFileInfoToJSON(std::string fileName);

#endif // _DEBUG
private:
	std::string m_sFilePath;
	std::string m_sFileContent;
	std::string m_sSelectedFile;

	std::vector<std::string> m_vSoundTypes = { "SFX", "MUSIC" };

	std::vector<SoundBanksList> m_vSoundBanksList; // list of Sound Bank Lists
	std::vector<JSONSoundFile> m_vSoundFileInfo;
	//std::map<std::string, std::vector<JSONSoundFile>> m_vSoundFileData; // list of Sound Files for a given Sound Bank

	bool m_bSoundBankToLoad;
	//bool m_bIsSoundBankReloaded;
	int m_iActiveSoundBankIndex;
};

#endif // AUDIOEDITOR_H