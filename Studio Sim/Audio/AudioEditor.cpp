#include "stdafx.h"
#include "AudioEditor.h"

AudioEditor::AudioEditor() {
	m_iDefaultVolume = 1.0f;
	m_iActiveSoundBank = MUSIC;
	m_bChangedSoundBankType = false;
	m_selectedSoundBankFile = AudioEngine::GetInstance()->GetSoundBank(MUSIC)->at(1);
	m_iDefaultVolume = AudioEngine::GetInstance()->GetSoundBank(MUSIC)->at(1)->volume;
}

#if _DEBUG
void AudioEditor::SpawnControlWindow()
{
	if (ImGui::Begin("Audio Editor", FALSE, ImGuiWindowFlags_AlwaysAutoResize))
	{
		Play();
		Unpause();
		Pause();
		Stop();
		Save();
		SaveNew();
		Load();
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
	UpdateVolumeSlider();
}

void AudioEditor::Play()
{
	if (ImGui::Button("Play")) {
		// Play chosen audio from the soundbank list
		AudioEngine::GetInstance()->PlayAudio(L"partymusic", MUSIC);
	}
}

void AudioEditor::Unpause()
{
	if (ImGui::Button("Unpause")) {
		// Play chosen audio from the soundbank list
		AudioEngine::GetInstance()->UnpauseMusic();
	}
}

void AudioEditor::Pause()
{
	if (ImGui::Button("Pause Music"))
	{
		// Pause the music
		AudioEngine::GetInstance()->PauseMusic();
	}
}

void AudioEditor::Stop()
{
	if (ImGui::Button("Stop Music"))
	{
		// Stop the music
		AudioEngine::GetInstance()->StopMusic();
	}
}

void AudioEditor::Save()
{
	if (ImGui::Button("Save to JSON"))
	{
		// Save the sound bank information into JSON

	}
}

void AudioEditor::SaveNew()
{
	if (ImGui::Button("Save New to JSON"))
	{
		// Save the sound bank information into JSON, but new file

	}
}

void AudioEditor::Load()
{
	if (ImGui::Button("Load from JSON"))
	{
		// Load the sound bank information from JSON

	}
}

void AudioEditor::AddToSoundBank()
{
	if (ImGui::Button("Add to Sound Bank"))
	{
		// Add the chosen sound file into the sound bank

	}
}

void AudioEditor::DeleteFromSoundBank()
{
	if (ImGui::Button("Delete from Sound Bank"))
	{
		// Delete the chosen sound bank element from the sound bank list
	}
}

void AudioEditor::SoundBankType()
{
	// Choose sound bank type
	ImGui::Text("Sound Bank Type");
	static std::string previewSoundBank = "SFX";
	static const char* soundBankNameList[]{ "SFX", "Music" };
	
	if (ImGui::BeginCombo("##Sound Bank Type", previewSoundBank.c_str())) {
		for (int i = 0; i < ARRAYSIZE(soundBankNameList); i++) {
			const bool isSelected = i == m_iActiveSoundBank;
			if (ImGui::Selectable(soundBankNameList[i], isSelected)) {
				m_iActiveSoundBank = i;
				previewSoundBank = soundBankNameList[i];
			}
			
			switch (m_iActiveSoundBank) {
			case SFX:
				// Change sound bank list to SFX
				break;
			case MUSIC:
				// Change sound bank list to Music
				break;
			default:
				break;
			}
		}
		ImGui::EndCombo();
	}
}

void AudioEditor::SoundBankList()
{
	// Print active sound bank on the list, where you can choose specific sound to change values on (also add/remove from the soundbank list)
	if (m_bChangedSoundBankType) {
		std::vector<SoundBankFile*>* soundBank = AudioEngine::GetInstance()->GetSoundBank((AudioType)m_iActiveSoundBank);
		for (int i = 0; soundBank->size() > i; i++) {
			// print sounds on list
			
		}

		m_bChangedSoundBankType = false;
	}

}

void AudioEditor::VolumeSlider()
{
	// On choice of sound bank, update default volume to set up value in a sound bank
	ImGui::Text("Default Volume");
	ImGui::SliderFloat("##Default Volume", &m_iDefaultVolume, 0.0f, 1.0f);
}

void AudioEditor::UpdateVolumeSlider()
{
	m_selectedSoundBankFile->volume = m_iDefaultVolume;
}






#endif // DEBUG