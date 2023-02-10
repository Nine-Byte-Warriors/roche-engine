#pragma once
#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include <xaudio2.h>
#include <thread>
#include <mutex>
#include <filesystem>

#include <nlohmann/json.hpp>
#include <JsonLoading.h>
//#include "VoiceCallback.h"

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
	std::vector<std::shared_ptr<SoundBankFile>>* soundBankAddress;
	std::string soundBankName;
	std::wstring fileName;
	std::string tagName;
	XAUDIO2_BUFFER* buffer;
	WAVEFORMATEX* sourceFormat;
	//VoiceCallback* voiceCallback;
	float volume;
	bool randomPitch = false;
	float pitchMin = 1.0f;
	float pitchMax = 1.0f;
};

struct JSONSoundFile {
	std::string name;
	std::string filePath;
	std::string tagName;
	float volume;
	int audioType;
	bool randomPitch;
	float pitchMin;
	float pitchMax;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(JSONSoundFile, name, filePath, tagName, volume, audioType, randomPitch, pitchMin, pitchMax);

struct SoundBanksList // these are the lists to hold file paths to different sound bank jsons
{
	std::string name;
	std::string filePath;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SoundBanksList, name, filePath)

enum AudioType
{
	SFX = 0,
	MUSIC = 1
};

class AudioEngine
{
public:
	AudioEngine();
	~AudioEngine();

	AudioEngine(AudioEngine& otherEngine) = delete;
	void operator=(const AudioEngine&) = delete;

	static AudioEngine* GetInstance();

	void Initialize(float masterVolume, float musicVolume, float sfxVolume, int maxMusicSourceVoices, int maxSFXSourceVoices);
	void Update(); // keep it on separate thread ideally

	void LoadAudioFromJSON(std::string loadFilePath); //JSON Pre-loading Function
	void SaveAudioToJSON(std::vector<std::shared_ptr<SoundBankFile>>& sfxSoundList, std::vector<std::shared_ptr<SoundBankFile>>& musicSoundList, std::string fileName); // JSON Save Function

	void LoadSoundBanksList(std::string loadFilePath);
	HRESULT LoadAudio(std::string soundBankName, std::wstring filePath, std::string tagName, float volume, AudioType audioType, bool randomPitchEnabled, float pitchMinimum, float pitchMaximum); // supports *.wav format only
	HRESULT PlayAudio(std::string soundBankName, std::string tagName, AudioType audioType);
	HRESULT UnpauseMusic(); // Unpauses ALL music
	HRESULT PauseMusic(); // Pauses ALL music
	HRESULT StopMusic(); // Stops ALL music and removes it from music source voice list
	HRESULT UnloadAudio(std::wstring fileName, std::string soundBankName, AudioType audioType); // Unloading audio from sound bank list
	void UnloadAllAudio();
	void StopAllAudio();

	// Loading Audio stuff
	HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset);

	// Deleting Audio
	void DeleteAudioData(std::shared_ptr<SoundBankFile> soundBankFile);

	std::shared_ptr<SoundBankFile> CreateSoundBankFile(std::wstring filePath, std::string tagName, XAUDIO2_BUFFER* buffer, WAVEFORMATEX* waveformatex, float volume, bool randomPitchEnabled, float pitchMinimum, float pitchMaximum);
	void AddToSoundBank(std::shared_ptr<SoundBankFile> soundBankFile, std::vector<std::shared_ptr<SoundBankFile>>& soundBank);
	void CheckSoundBankExistence(std::string soundBankName, AudioType audioType);
	std::vector<std::shared_ptr<SoundBankFile>>& GetSoundBank(std::string soundBankName, AudioType audioType);
	std::shared_ptr<SoundBankFile> FindSoundBankFile(std::wstring fileName, std::string soundBankName, AudioType audioType);

	std::wstring GetFileName(std::wstring filePath);
	std::wstring GetFileName(std::string filePath);
	std::string GetFileNameString(std::wstring filePath);
	std::string GetFileNameString(std::string filePath);

	inline std::vector<std::string> GetSoundBankNamesList() { return m_vSoundBankNamesList; };

	// Volume controls - these are taken into consideration when playing audio, alongside with master volume
	// Master volume has its own set of functions to control (use master voice for this)
	inline float GetMasterVolume() { return m_fMasterVolume; }
	inline void SetMasterVolume(float masterVolume) {
		m_fMasterVolume = masterVolume;
		m_pMasterVoice->SetVolume(m_fMasterVolume);
	};
	inline float GetMusicVolume() { return m_fMusicVolume; }
	inline void SetMusicVolume(float musicVolume) {
		m_fMusicVolume = musicVolume;
		m_pMusicSubmixVoice->SetVolume(m_fMusicVolume);
	};
	inline float GetSFXVolume() { return m_fSFXVolume; }
	inline void SetSFXVolume(float sfxVolume) {
		m_fSFXVolume = sfxVolume;
		m_pSFXSubmixVoice->SetVolume(m_fSFXVolume);
	};
	inline void SetDefaultVolume(std::shared_ptr<SoundBankFile> soundBank, float newVolume) { soundBank->volume = newVolume; };
	inline void SetRandomPitch(std::shared_ptr<SoundBankFile> soundBank) { soundBank->randomPitch = !soundBank->randomPitch; };
	inline void SetPitchMin(std::shared_ptr<SoundBankFile> soundBank, float newPitchMin) { soundBank->pitchMin = newPitchMin; };
	inline void SetPitchMax(std::shared_ptr<SoundBankFile> soundBank, float newPitchMax) { soundBank->pitchMax = newPitchMax; };

private:
	IXAudio2* m_pXAudio2; // XAudio2 audio engine instance
	IXAudio2MasteringVoice* m_pMasterVoice;

	IXAudio2SubmixVoice* m_pSFXSubmixVoice;
	XAUDIO2_SEND_DESCRIPTOR* m_pSFXSend;
	XAUDIO2_VOICE_SENDS* m_pSFXSendList;

	IXAudio2SubmixVoice* m_pMusicSubmixVoice;
	XAUDIO2_SEND_DESCRIPTOR* m_pMusicSend;
	XAUDIO2_VOICE_SENDS* m_pMusicSendList;

	// For singleton
	static AudioEngine* m_pAudioEngineInstance;
	static std::mutex m_mutex;

	std::vector<std::string> m_vSoundBankNamesList;

	std::map<std::string, std::vector<std::shared_ptr<SoundBankFile>>> m_SFXSoundBankMap;
	std::map<std::string, std::vector<std::shared_ptr<SoundBankFile>>> m_MusicSoundBankMap;

	std::vector<IXAudio2SourceVoice*> m_vMusicSourceVoiceList;
	std::vector<IXAudio2SourceVoice*> m_vSFXSourceVoiceList;

	int m_iMaxSFXSourceVoicesLimit;
	int m_iMaxMusicSourceVoicesLimit;

	float m_fMasterVolume;
	float m_fMusicVolume;
	float m_fSFXVolume;

	bool m_bIsMusicPaused;

};

#endif // AUDIOENGINE_H