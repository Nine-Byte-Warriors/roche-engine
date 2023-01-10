#pragma once
#include <xaudio2.h>
#include <thread>
#include <mutex>
#include <stdafx.h>

//#include <tchar.h>
//#include <winnt.h>

//#include "mmeapi.h"

// Windows uses Little-Endian
#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'

struct SoundBankFile {
	std::wstring fileName;
	XAUDIO2_BUFFER* buffer;
	float volume;
	int priority;
};

struct SourceVoiceList {
	IXAudio2SourceVoice* sourceVoice;
	int priority;
};

class AudioEngine {
public:
	AudioEngine();

	//AudioEngine(AudioEngine& otherEngine) = delete;
	//void operator=(const AudioEngine&) = delete;

	static AudioEngine* GetInstance();

	void Initialize(int numberOfSFXSourceVoices, int numberOfMusicSourceVoices);
	void Update(float deltaTime);

	// Bare minimum requirements
	HRESULT LoadAudio(std::wstring filePath);
	HRESULT PlayAudio(std::wstring fileName);
	void PauseAudio();
	void UnloadAudio();

	//// Loading Audio stuff
	// TODO
	// Allocate this stuff in some buffers container?
	HRESULT ParseAudio(std::wstring filePath, IXAudio2SourceVoice* sourceVoice);
	HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset);




	// Event System TBD if we want to even use it
	//void HandleEvent(Event* event);

private:
	IXAudio2* m_pXAudio2; // XAudio2 audio engine instance
	IXAudio2MasteringVoice* m_pMasterVoice;

	// For singleton
	static AudioEngine* m_pAudioEngineInstance;
	static std::mutex m_mutex;

	//TEST
	IXAudio2SourceVoice* pSourceVoice;
	IXAudio2SourceVoice* pSourceVoice2;

	std::vector<SoundBankFile*> m_vMusicSoundBank; // Music Sound Bank
	std::vector<SoundBankFile*> m_vSFXSoundBank; // SFX Sound Bank

	std::vector<SourceVoiceList*> m_pMusicSourceVoices; // Music Source Voices
	std::vector<SourceVoiceList*> m_vSFXSourceVoices; // Channels Source Voice


};