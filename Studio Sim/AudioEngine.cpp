#include "stdafx.h"
#include "AudioEngine.h"

AudioEngine* AudioEngine::m_pAudioEngineInstance{ nullptr };
std::mutex AudioEngine::m_mutex;

AudioEngine::AudioEngine() : m_pXAudio2(nullptr), m_pMasterVoice(nullptr) {

}

AudioEngine* AudioEngine::GetInstance()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	if (m_pAudioEngineInstance == nullptr) {
		m_pAudioEngineInstance = new AudioEngine();
	}
	return m_pAudioEngineInstance;
}

void AudioEngine::Initialize()
{
	HRESULT hr;

	// Creates instance of XAudio2 engine
	if (FAILED(hr = XAudio2Create(&m_pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR))) {
		ErrorLogger::Log(hr, "AudioEngine: Failed to create XAudio2 engine.");
	}

	// Master voice encapsulates and audio device, the final destination where all audio sources go
	if (FAILED(hr = m_pXAudio2->CreateMasteringVoice(&m_pMasterVoice))) {
		ErrorLogger::Log(hr, "AudioEngine: Failed to create mastering voice.");
	}

	

}





//void AudioEngine::HandleEvent(Event* event)
//{
//	switch (event->GetEventID()) {
//		case EVENTID::LoadAudioEvent: {
//			// Load Audio
//		}
//		break;
//		case EVENTID::PlayAudioEvent: {
//			// Play Audio
//		}
//		break;
//		case EVENTID::StopAudioEvent: {
//			// Stop Audio
//		}
//		break;
//		case EVENTID::UnloadAudioEvent: {
//			// Unload Audio
//		}
//		break;
//	}
//}
