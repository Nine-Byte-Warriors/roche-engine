#pragma once
#ifndef AUDIOEDITOR_H

#include "structures.h"
#include "AudioEngine.h"

#if _DEBUG
#include <imgui/imgui.h>
#endif

class AudioEditor {
public:
	AudioEditor();

#if _DEBUG
	void SpawnControlWindow();
#endif

	void Update();




private:
	// Window functions:
	void Play();
	void Pause();
	void Stop();

	// to be added file path and stuff
	void Save();
	void SaveNew();
	void Load();

	void AddToSoundBank();
	void DeleteFromSoundBank();

	// Sound list handle
	void SoundBankType();
	void SoundBankList();

	// Parameter sliders
	void VolumeSlider();



	// Audio controls
	bool m_bPlayButton;
	bool m_bPauseButton;
	bool m_bStopButton;
	
	// Sound Bank controls
	bool m_bAddButton;
	bool m_bDeleteButton;

	// Saving/Loading fields ----- to be done
	//std::wstring m_sSelectedFile = L"LoadFile.txt";
	//std::wstring m_sFilePath = L"";
	//std::wstring m_sFileContent = L"";

	// Save/Load controls
	bool m_bSaveButton;
	bool m_bSaveNewButton;
	bool m_bLoadButton;


	// List of audio



#if _DEBUG

	//const ImVec2 m_vImageButtonSize = ImVec2(32, 32);
#endif




};



#endif // AUDIOEDITOR_H
