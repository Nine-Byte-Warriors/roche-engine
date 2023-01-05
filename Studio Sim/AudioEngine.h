#pragma once
#include <xaudio2.h>
#include <thread>
#include <mutex>

#include <tchar.h>

#define fourccRIFF 'RIFF'
#define fourccWAVE 'WAVE'

//#include "EventSystem.h" // remove if not using event system

class AudioEngine {
public:
	AudioEngine();

	//AudioEngine(AudioEngine& otherEngine) = delete;
	//void operator=(const AudioEngine&) = delete;

	static AudioEngine* GetInstance();

	void Initialize();

	// Bare minimum requirements
	void LoadAudio();
	void PlayAudio();
	void PauseAudio();
	void UnloadAudio();

	////
	HRESULT ParseAudio();
	HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkData);
	HRESULT ReadChunkData(HANDLE hFile, void* filetype, DWORD buffersize, DWORD bufferoffset);

	// Event System TBD if we want to even use it
	//void HandleEvent(Event* event);

private:
	IXAudio2* m_pXAudio2; // XAudio2 audio engine instance
	IXAudio2MasteringVoice* m_pMasterVoice;


	// For singleton
	static AudioEngine* m_pAudioEngineInstance;
	static std::mutex m_mutex;
};