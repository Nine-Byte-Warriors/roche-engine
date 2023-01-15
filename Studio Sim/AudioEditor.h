#pragma once
#ifndef AUDIOEDITOR_H

#include "structures.h"

#if _DEBUG
#include <imgui/imgui.h>
#endif

class AudioEditor {
public:
	AudioEditor();

#if _DEBUG
	void SpawnControlWindow();
#endif






private:
	// Window functions
	void Play();
	void Pause();
	void Stop();

	void Load();
	void Unload();

	// Sound Bank Lists:


	// Saving/Loading ----- to be done
	//bool m_bSaveButton;
	//bool m_bSaveNewButton;

	//bool m_bLoadButton;
	//std::string m_sSelectedFile = "LoadFile.txt";
	//std::string m_sFilePath = "";
	//std::string m_sFileContent = "";

	// Audio controls
	bool m_bPlayButton;
	bool m_bPlayOnce;
	bool m_bPauseButton;
	bool m_bStopButton;

	// List of audio



#if _DEBUG

	//const ImVec2 m_vImageButtonSize = ImVec2(32, 32);
	//const ImVec2 m_vImageButtonFrame0 = ImVec2(10, 10);
	//const ImVec2 m_vImageButtonFrame1 = ImVec2(10, 10);
#endif




};



#endif // AUDIOEDITOR_H
