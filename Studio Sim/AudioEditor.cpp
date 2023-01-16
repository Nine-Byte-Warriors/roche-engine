#include <stdafx.h>
#include "AudioEditor.h"

AudioEditor::AudioEditor() {

}

#if _DEBUG

void AudioEditor::SpawnControlWindow()
{
	if (ImGui::Begin("Audio Editor", FALSE, ImGuiWindowFlags_AlwaysAutoResize))
	{
		Play();
		Pause();
		Stop();
		//Save();
		//SaveNew();
		//Load();
		SoundBankType();
		SoundBankList();
		VolumeSlider();
		AddToSoundBank();
		DeleteFromSoundBank();
	}

	ImGui::End();
}

void AudioEditor::Update()
{
	if (m_bPlayButton) {
		AudioEngine::GetInstance()->PlayAudio(L"bullettest", SFX);
		m_bPlayButton == false;
	}



}

void AudioEditor::Play()
{
	m_bPlayButton = ImGui::Button("Play");
	if (m_bPlayButton)
	{
		m_bPlayButton = true;
	}
}

void AudioEditor::Pause()
{
	m_bPauseButton = ImGui::Button("Pause Music");
	if (m_bPauseButton)
	{
		m_bPauseButton = true;
	}
}

void AudioEditor::Stop()
{
	m_bStopButton = ImGui::Button("Stop Music");
	if (m_bStopButton)
	{
		m_bStopButton = true;
	}
}

void AudioEditor::Save()
{
	m_bSaveButton = ImGui::Button("Save");
	if (m_bSaveButton)
	{
		m_bSaveButton = true;
	}
}

void AudioEditor::SaveNew()
{
	m_bSaveNewButton = ImGui::Button("Save New");
	if (m_bSaveNewButton)
	{
		m_bSaveNewButton = true;
	}
}

void AudioEditor::Load()
{
	m_bLoadButton = ImGui::Button("Load");
	if (m_bLoadButton)
	{
		m_bLoadButton = true;
	}
}

void AudioEditor::AddToSoundBank()
{
	m_bAddButton = ImGui::Button("Add");
	if (m_bAddButton)
	{
		m_bAddButton = true;
	}
}

void AudioEditor::DeleteFromSoundBank()
{
	m_bDeleteButton = ImGui::Button("Delete");
	if (m_bDeleteButton)
	{
		m_bDeleteButton = true;
	}
}

void AudioEditor::SoundBankType()
{
	ImGui::Text("Sound Bank Type");
	int activeType = 0;
	
	if (ImGui::BeginCombo("Sound Bank Type", 0));
}

void AudioEditor::SoundBankList()
{
}

void AudioEditor::VolumeSlider()
{

}

#endif // DEBUG