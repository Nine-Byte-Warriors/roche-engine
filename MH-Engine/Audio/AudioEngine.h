#pragma once
#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include <xaudio2.h>
#include <thread>
#include <mutex>
//#include <stdafx.h>
#include <filesystem>

#include <nlohmann/json.hpp>
#include <JsonLoading.h>

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

struct JSONSoundFile {
	std::string name;
	std::string filePath;
	float volume;
	int audioType;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(JSONSoundFile, name, filePath, volume, audioType);

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

	void LoadAudioFromJSON(std::string loadFilePath); //JSON Pre-loading Function
	void SaveAudioToJSON(std::vector<SoundBankFile*>* sfxSoundList, std::vector<SoundBankFile*>* musicSoundList, std::string fileName); // JSON Save Function

	HRESULT LoadAudio(std::wstring filePath, float volume, AudioType audioType); // supports *.wav format only
	HRESULT PlayAudio(std::wstring fileName, AudioType audioType);
	HRESULT UnpauseMusic(); // Unpauses ALL music
	HRESULT PauseMusic(); // Pauses ALL music
	HRESULT StopMusic(); // Stops ALL music and removes it from music source voice list
	HRESULT UnloadAudio(std::wstring fileName, AudioType audioType); // Unloading audio from sound bank list
	void StopAllAudio();

	//// Loading Audio stuff
	HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset);

	SoundBankFile* CreateSoundBankFile(std::wstring filePath, XAUDIO2_BUFFER* buffer, WAVEFORMATEX* waveformatex, float volume);
	void AddToSoundBank(SoundBankFile* soundBankFile, std::vector<SoundBankFile*>* soundBank);
	std::wstring GetFileName(std::wstring filePath);
	std::wstring GetFileName(std::string filePath); //overload for string

	std::vector<SoundBankFile*>* GetSoundBank(AudioType audioType);
	
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
	inline void SetDefaultVolume(SoundBankFile* soundBank, float defaultVolume) { soundBank->volume = defaultVolume; };

	SoundBankFile* FindSoundBankFile(std::wstring fileName, AudioType audioType);

private:
	IXAudio2* m_pXAudio2; // XAudio2 audio engine instance
	IXAudio2MasteringVoice* m_pMasterVoice;

	// For singleton
	static AudioEngine* m_pAudioEngineInstance;
	static std::mutex m_mutex;

	IXAudio2SourceVoice* pSourceVoice;
	IXAudio2SourceVoice* pSourceVoice2;

	std::vector<SoundBankFile*>* m_vMusicSoundBank; // Music Sound Bank
	std::vector<SoundBankFile*>* m_vSFXSoundBank; // SFX Sound Bank

	std::vector<IXAudio2SourceVoice*>* m_vMusicSourceVoiceList;
	std::vector<IXAudio2SourceVoice*>* m_vSFXSourceVoiceList;

	int m_iMaxSFXSourceVoicesLimit;
	int m_iMaxMusicSourceVoicesLimit;

	float m_fMasterVolume;
	float m_fMusicVolume;
	float m_fSFXVolume;

	bool m_bIsMusicPaused;
};

#endif // VOICECALLBACK_H