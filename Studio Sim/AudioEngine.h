#pragma once
#include <xaudio2.h>
#include <thread>
#include <mutex>
#include <stdafx.h>
#include <filesystem>

#include "VoiceCallback.h"
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
	WAVEFORMATEX* sourceFormat;
	VoiceCallback* voiceCallback;
	float volume;
};

enum AudioType {
	SFX = 0,
	MUSIC = 1
};

class AudioEngine {
public:
	AudioEngine();

	//AudioEngine(AudioEngine& otherEngine) = delete;
	//void operator=(const AudioEngine&) = delete;

	static AudioEngine* GetInstance();

	void Initialize(int maxMusicSourceVoices, int maxSFXSourceVoices);
	void Update(float deltaTime);

	// Bare minimum requirements
	HRESULT LoadAudio(std::wstring filePath, float volume, AudioType audioType);
	HRESULT PlayAudio(std::wstring fileName, AudioType audioType);
	HRESULT UnloadAudio(std::wstring fileName, AudioType audioType);

	HRESULT StopMusic();


	//// Loading Audio stuff
	HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset);

	// TODO
	// Create soundbank
	// Helper function to get a fileName
	SoundBankFile* CreateSoundBankFile(std::wstring filePath, XAUDIO2_BUFFER* buffer, WAVEFORMATEX* waveformatex, float volume);
	void AddToSoundBank(SoundBankFile* soundBankFile, std::vector<SoundBankFile*>* soundBank);
	std::wstring GetFileName(std::wstring filePath);

	std::vector<SoundBankFile*>* GetSoundBank(AudioType audioType);
	
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

	std::vector<SoundBankFile*>* m_vMusicSoundBank; // Music Sound Bank
	std::vector<SoundBankFile*>* m_vSFXSoundBank; // SFX Sound Bank

	int m_iMusicSourceVoiceLimit;
	int m_iSFXSourceVoiceLimit;

	int m_iMaxSFXSourceVoicesLimit;
	int m_iMaxMusicSourceVoicesLimit;

	int m_iSFXSourceVoicesPlaying;
	int m_iMusicSourceVoicesPlaying;
};