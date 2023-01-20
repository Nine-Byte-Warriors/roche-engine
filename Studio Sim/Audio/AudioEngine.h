#pragma once
#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include <xaudio2.h>
#include <thread>
#include <mutex>
//#include <stdafx.h>
#include <filesystem>

//#ifndef VOICECALLBACK_H
//#define VOICECALLBACK_H
//#include "VoiceCallback.h"


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

class VoiceCallback;

struct SoundBankFile
{
	std::wstring fileName;
	XAUDIO2_BUFFER* buffer;
	WAVEFORMATEX* sourceFormat;
	//VoiceCallback* voiceCallback;
	float volume;
};

enum AudioType
{
	SFX = 0,
	MUSIC = 1
};

class AudioEngine
{
public:
	AudioEngine();

	AudioEngine(AudioEngine& otherEngine) = delete;
	void operator=(const AudioEngine&) = delete;

	static AudioEngine* GetInstance();

	void Initialize(float masterVolume, float musicVolume, float sfxVolume, int maxMusicSourceVoices, int maxSFXSourceVoices);
	void Update(); // keep it on separate thread

	HRESULT LoadAudio(std::wstring filePath, float volume, AudioType audioType); // supports *.wav format only
	HRESULT PlayAudio(std::wstring fileName, AudioType audioType);
	HRESULT UnpauseMusic(); // Unpauses ALL music
	HRESULT PauseMusic(); // Pauses ALL music
	HRESULT StopMusic(); // Stops ALL music and removes it from music source voice list
	HRESULT UnloadAudio(std::wstring fileName, AudioType audioType); // Unloading audio from sound bank list

	//// Loading Audio stuff
	HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset);

	SoundBankFile* CreateSoundBankFile(std::wstring filePath, XAUDIO2_BUFFER* buffer, WAVEFORMATEX* waveformatex, float volume);
	void AddToSoundBank(SoundBankFile* soundBankFile, std::vector<SoundBankFile*>* soundBank);
	std::wstring GetFileName(std::wstring filePath);

	std::vector<SoundBankFile*>* GetSoundBank(AudioType audioType);
	
	// Event System TBD if we want to even use it
	//void HandleEvent(Event* event);

	// Volume controls - these are taken into consideration when playing audio, alongside with master volume
	// Master volume has its own set of functions to control (use master voice for this)
	inline float GetMasterVolume() { return m_fMasterVolume; }
	inline void SetMasterVolume(float masterVolume) { 
		m_fMasterVolume = masterVolume; 
		m_pMasterVoice->SetVolume(m_fMasterVolume);
	};
	inline float GetMusicVolume() { return m_fMusicVolume; }
	inline void SetMusicVolume(float musicVolume) { m_fMusicVolume = musicVolume; };
	inline float GetSFXVolume() { return m_fSFXVolume; }
	inline void SetSFXVolume(float sfxVolume) { m_fSFXVolume = sfxVolume; };

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

	std::vector<IXAudio2SourceVoice*>* m_vMusicSourceVoiceList;
	std::vector<IXAudio2SourceVoice*>* m_vSFXSourceVoiceList;

	int m_iMaxSFXSourceVoicesLimit;
	int m_iMaxMusicSourceVoicesLimit;

	//int m_iSFXSourceVoicesPlaying;
	//int m_iMusicSourceVoicesPlaying;

	float m_fMasterVolume;
	float m_fMusicVolume;
	float m_fSFXVolume;

	bool m_bIsMusicPaused;
};

#endif // VOICECALLBACK_H