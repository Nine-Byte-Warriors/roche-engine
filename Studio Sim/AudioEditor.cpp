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
	}

	ImGui::End();
}

#endif

void AudioEditor::Play()
{
	m_bPlayButton = ImGui::Button("Play");
	if (m_bPlayButton)
	{
		m_bPlayOnce = true;
	}
}

void AudioEditor::Pause()
{
}

void AudioEditor::Stop()
{
}

void AudioEditor::Load()
{
}

void AudioEditor::Unload()
{
}
