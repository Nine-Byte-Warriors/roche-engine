#pragma once
#include <xaudio2.h>
#include <thread>
#include <mutex>

#define fourccWAVE 'EVAW'

//#include "EventSystem.h" // remove if not using event system

class AudioEngine {
public:
	AudioEngine();

	//AudioEngine(AudioEngine& otherEngine) = delete;
	//void operator=(const AudioEngine&) = delete;

	static AudioEngine* GetInstance();

	void Initialize();

	////


	// Event System TBD if we want to even use it
	//void HandleEvent(Event* event);

private:
	IXAudio2* m_pXAudio2; // XAudio2 audio engine instance
	IXAudio2MasteringVoice* m_pMasterVoice;


	// For singleton
	static AudioEngine* m_pAudioEngineInstance;
	static std::mutex m_mutex;
};