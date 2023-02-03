#include "stdafx.h"
#include "AudioEngine.h"
#include <RND.h>

AudioEngine* AudioEngine::m_pAudioEngineInstance{ nullptr };
std::mutex AudioEngine::m_mutex;

AudioEngine::AudioEngine() : m_pXAudio2(nullptr), m_pMasterVoice(nullptr) {
	// REMOVE WHEN WE WILL ACTUALLY HANDLE IT FROM THE LEVEL EDITOR SIDE
	m_vSoundBankNamesList.push_back("SoundBankTest1");
	m_vSoundBankNamesList.push_back("SoundBankTest2");
	m_vSoundBankNamesList.push_back("SoundBankTest3");
}

AudioEngine::~AudioEngine()
{

}

AudioEngine* AudioEngine::GetInstance()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	if (m_pAudioEngineInstance == nullptr) {
		m_pAudioEngineInstance = new AudioEngine();
	}

	return m_pAudioEngineInstance;
}

void AudioEngine::Initialize(float masterVolume, float musicVolume, float sfxVolume, int maxMusicSourceVoices, int maxSFXSourceVoices)
{
	HRESULT hr;

	// Creates instance of XAudio2 engine
	if (FAILED(hr = XAudio2Create(&m_pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR))) {
		ErrorLogger::Log(hr, "AudioEngine::Initialize: Failed to create XAudio2 engine.");
	}

	// Master voice encapsulates and audio device, the final destination where all audio sources go
	if (FAILED(hr = m_pXAudio2->CreateMasteringVoice(&m_pMasterVoice))) {
		ErrorLogger::Log(hr, "AudioEngine::Initialize: Failed to create mastering voice.");
	}

	m_fMasterVolume = masterVolume;
	m_fMusicVolume = musicVolume;
	m_fSFXVolume = sfxVolume;

	m_iMaxSFXSourceVoicesLimit = maxSFXSourceVoices;
	m_iMaxMusicSourceVoicesLimit = maxMusicSourceVoices;
}

void AudioEngine::Update()
{
	XAUDIO2_VOICE_STATE state;

	for (int i = 0; m_vSFXSourceVoiceList.size() > i; i++) {
		if (m_vSFXSourceVoiceList.at(i)->GetState(&state), state.BuffersQueued <= 0)
		{
			m_vSFXSourceVoiceList.at(i)->DestroyVoice();
			m_vSFXSourceVoiceList.erase(m_vSFXSourceVoiceList.begin() + i	);
		}
	}
}

void AudioEngine::LoadAudioFromJSON(std::string loadFilePath)
{
	// Load Json File from given file path
	std::vector<JSONSoundFile> soundFileListToLoad;
	JsonLoading::LoadJson(soundFileListToLoad, loadFilePath);

	// Load audio into Sound Banks
	for (int i = 0; soundFileListToLoad.size() > i; i++) {
		LoadAudio(GetFileNameString(loadFilePath), StringHelper::StringToWide(soundFileListToLoad.at(i).filePath), soundFileListToLoad.at(i).tagName, soundFileListToLoad.at(i).volume, (AudioType)soundFileListToLoad.at(i).audioType, 
									soundFileListToLoad.at(i).randomPitch, soundFileListToLoad.at(i).pitchMin, soundFileListToLoad.at(i).pitchMax);
	}
}

void AudioEngine::SaveAudioToJSON(std::vector<std::shared_ptr<SoundBankFile>>& sfxSoundList, std::vector<std::shared_ptr<SoundBankFile>>& musicSoundList, std::string fileName)
{
	std::vector<JSONSoundFile> soundFileList;
	std::string name;

	for (int i = 0; sfxSoundList.size() > i; i++) {
		name = StringHelper::StringToNarrow(sfxSoundList.at(i)->fileName);
		soundFileList.push_back({ name, "Resources\\Audio\\" + name + ".wav", sfxSoundList.at(i)->tagName, sfxSoundList.at(i)->volume, 0,
			sfxSoundList.at(i)->randomPitch, sfxSoundList.at(i)->pitchMin, sfxSoundList.at(i)->pitchMax });
	}

	for (int i = 0; musicSoundList.size() > i; i++) {
		name = StringHelper::StringToNarrow(musicSoundList.at(i)->fileName);
		soundFileList.push_back({ name, "Resources\\Audio\\" + name + ".wav", musicSoundList.at(i)->tagName, musicSoundList.at(i)->volume, 1,
			musicSoundList.at(i)->randomPitch, musicSoundList.at(i)->pitchMin, musicSoundList.at(i)->pitchMax });
	}

	JsonLoading::SaveJson(soundFileList, "Resources\\Audio\\Sound Banks\\" + fileName + ".json");
}

void AudioEngine::LoadSoundBanksList(std::string loadFilePath)
{
	m_vSoundBankNamesList.clear();
	std::vector<SoundBanksList> soundBanksList;
	JsonLoading::LoadJson(soundBanksList, loadFilePath);
	for (int i = 0; soundBanksList.size() > i; i++) {
		LoadAudioFromJSON(soundBanksList.at(i).filePath);
		m_vSoundBankNamesList.push_back(soundBanksList.at(i).name);
	}
}

HRESULT AudioEngine::LoadAudio(std::string soundBankName, std::wstring filePath, std::string tagName, float volume, AudioType audioType, bool randomPitchEnabled, float pitchMinimum, float pitchMaximum)
{
	HRESULT hr = S_OK;

	//// Create structures
	WAVEFORMATEXTENSIBLE* wfx = new WAVEFORMATEXTENSIBLE();
	XAUDIO2_BUFFER* buffer = new XAUDIO2_BUFFER();

	//// Open audio with CreateFile
	const wchar_t* strFileName = filePath.c_str();

	// Open file
	HANDLE hFile = CreateFile(
		strFileName,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);

	if (INVALID_HANDLE_VALUE == hFile) {
		hr = HRESULT_FROM_WIN32(GetLastError());
		ErrorLogger::Log(hr, "AudioEngine::LoadAudio: INVALID_HANDLE_VALUE Error");
		return hr;
	}

	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN)) {
		hr = HRESULT_FROM_WIN32(GetLastError());
		ErrorLogger::Log(hr, "AudioEngine::LoadAudio: INVALID_SET_FILE_POINTER Error");
		return hr;
	}

	// Find RIFF chunk and check the file type
	DWORD dwChunkSize;
	DWORD dwChunkPosition;
	//check the file type, should be fourccWAVE or 'XWMA'
	FindChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
	DWORD filetype;
	ReadChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);

	if (filetype != fourccWAVE) {
		hr = S_FALSE;
		ErrorLogger::Log(hr, "AudioEngine::LoadAudio: File type is not WAVE");
		return hr;
	}

	FindChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
	ReadChunkData(hFile, wfx, dwChunkSize, dwChunkPosition);

	//fill out the audio data buffer with the contents of the fourccDATA chunk
	FindChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
	BYTE* pDataBuffer = new BYTE[dwChunkSize];
	ReadChunkData(hFile, pDataBuffer, dwChunkSize, dwChunkPosition);

	buffer->AudioBytes = dwChunkSize;  // size of the audio buffer in bytes
	buffer->pAudioData = pDataBuffer;  // buffer containing audio data
	buffer->Flags = XAUDIO2_END_OF_STREAM; // tell the source voice not to expect any data after this buffer

	CheckSoundBankExistence(soundBankName, audioType);

	std::vector<std::shared_ptr<SoundBankFile>>* soundBank = &GetSoundBank(soundBankName, audioType);
	std::shared_ptr<SoundBankFile> soundBankFile = CreateSoundBankFile(filePath, tagName, buffer, (WAVEFORMATEX*)wfx, volume, randomPitchEnabled, pitchMinimum, pitchMaximum);

	AddToSoundBank(soundBankFile, *soundBank);

	return hr;
}

HRESULT AudioEngine::PlayAudio(std::string soundBankName, std::string tagName, AudioType audioType)
{
	HRESULT hr = S_OK;
	float finalVolume;
	float frequencyRatio;
	float sourceRate;

	if (audioType == SFX && m_vSFXSourceVoiceList.size() >= m_iMaxSFXSourceVoicesLimit) {
		return S_FALSE;
	}
	else if (audioType == MUSIC && m_vMusicSourceVoiceList.size() >= m_iMaxMusicSourceVoicesLimit) {
		return S_FALSE;
	}

	IXAudio2SourceVoice* pVoice = nullptr;
	std::vector<std::shared_ptr<SoundBankFile>>& soundBank = GetSoundBank(soundBankName, audioType);

	// Check with the file name exists, if so, grab a buffer from it
	for (int i = 0; soundBank.size() > i; i++) {
		if (tagName == soundBank.at(i)->tagName) {

			if (FAILED(hr = m_pXAudio2->CreateSourceVoice(&pVoice, soundBank.at(i)->sourceFormat, 0, XAUDIO2_DEFAULT_FREQ_RATIO,
				NULL/*soundBank->at(i)->voiceCallback*/, NULL, NULL))) {
				ErrorLogger::Log(hr, "AudioEngine::PlayAudio: Failed to CreateSourceVoice");
				return hr;
			}

			if (FAILED(hr = pVoice->SubmitSourceBuffer(soundBank.at(i)->buffer))) {
				ErrorLogger::Log(hr, "AudioEngine::PlayAudio: Failed to SubmitSourceBuffer");
				return hr;
			}

			switch (audioType)
			{
			case SFX:
				finalVolume = m_fMasterVolume * m_fSFXVolume * soundBank.at(i)->volume;
				pVoice->SetVolume(finalVolume);
				if (soundBank.at(i)->randomPitch) {
					pVoice->GetFrequencyRatio(&sourceRate);
					frequencyRatio = sourceRate / RND::Getf(soundBank.at(i)->pitchMin, soundBank.at(i)->pitchMax);
					pVoice->SetFrequencyRatio(frequencyRatio);
				}
				m_vSFXSourceVoiceList.push_back(pVoice);
				break;
			case MUSIC:
				finalVolume = m_fMasterVolume * m_fMusicVolume * soundBank.at(i)->volume;
				pVoice->SetVolume(finalVolume);
				m_vMusicSourceVoiceList.push_back(pVoice);
				break;
			default:
				ErrorLogger::Log("AudioEngine::PlayAudio: Failed to add source voice to the list");
				break;
			}

			if (FAILED(hr = pVoice->Start(0))) {
				ErrorLogger::Log(hr, "AudioEngine::PlayAudio: Failed to start Source Voice");
				return hr;
			}
		}
	}

	return hr;
}

HRESULT AudioEngine::UnpauseMusic()
{
	HRESULT hr = S_OK;
	
	if (m_bIsMusicPaused) {
		for (int i = 0; m_vMusicSourceVoiceList.size() > i; i++) {
			m_vMusicSourceVoiceList.at(i)->Start(0);
		}

		m_bIsMusicPaused = false;
	}

	return hr;
}

HRESULT AudioEngine::PauseMusic()
{
	HRESULT hr = S_OK;

	for (int i = 0; m_vMusicSourceVoiceList.size() > i; i++) {
		if (FAILED(hr = m_vMusicSourceVoiceList.at(i)->Stop(0))) {
			ErrorLogger::Log(hr, "AudioEngine::PauseMusic: Failed to Stop music");
			return hr;
		}
	}

	m_bIsMusicPaused = true;

	return hr;
}

HRESULT AudioEngine::StopMusic()
{
	HRESULT hr = S_OK;

	for (int i = 0; m_vMusicSourceVoiceList.size() > i; i++) {
		if (FAILED(hr = m_vMusicSourceVoiceList.at(i)->Stop(0))) {
			ErrorLogger::Log(hr, "AudioEngine::StopMusic: Failed to Stop music");
			return hr;
		}
		else {
			m_vMusicSourceVoiceList.at(i)->DestroyVoice();
			m_vMusicSourceVoiceList.erase(m_vMusicSourceVoiceList.begin() + i);
		}
	}

	return hr;
}

HRESULT AudioEngine::UnloadAudio(std::wstring fileName, std::string soundBankName, AudioType audioType)
{
	HRESULT hr = S_OK;
	std::vector<std::shared_ptr<SoundBankFile>>& soundBank = GetSoundBank(soundBankName, audioType);

	for (int i = 0; soundBank.size() > i; i++) {
		if (fileName == soundBank.at(i)->fileName) {
			DeleteAudioData(soundBank.at(i));
			soundBank.erase(soundBank.begin() + i);

			return hr;
		}
	}

	hr = S_FALSE;
	ErrorLogger::Log(hr, "AudioEngine::UnloadAudio: Unsuccessful unload of the audio data");
	return hr;
}

void AudioEngine::UnloadAllAudio()
{
	StopAllAudio();

	while (m_SFXSoundBankMap.size() > 0) {
		for (const auto& [key, value] : m_SFXSoundBankMap) {
			while (m_SFXSoundBankMap[key].size() > 0) {
				DeleteAudioData(m_SFXSoundBankMap[key].at(0));
				m_SFXSoundBankMap[key].erase(m_SFXSoundBankMap[key].begin());
			}
		}
	}

	while (m_MusicSoundBankMap.size() > 0) {
		for (const auto& [key, value] : m_MusicSoundBankMap) {
			while (m_MusicSoundBankMap[key].size() > 0) {
				DeleteAudioData(m_MusicSoundBankMap[key].at(0));
				m_MusicSoundBankMap[key].erase(m_MusicSoundBankMap[key].begin());
			}
		}
	}
}

void AudioEngine::StopAllAudio()
{
	for (int i = m_vSFXSourceVoiceList.size(); m_vSFXSourceVoiceList.size() < i; i--) {
		m_vSFXSourceVoiceList.at(i)->DestroyVoice();
		m_vSFXSourceVoiceList.erase(m_vSFXSourceVoiceList.begin() + i);
	}

	for (int i = m_vSFXSourceVoiceList.size(); m_vMusicSourceVoiceList.size() < i; i--) {
		m_vMusicSourceVoiceList.at(i)->DestroyVoice();
		m_vMusicSourceVoiceList.erase(m_vMusicSourceVoiceList.end());
	}
}

HRESULT AudioEngine::FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN)) {
		hr = HRESULT_FROM_WIN32(GetLastError());
		ErrorLogger::Log(hr, "AudioEngine::FindChunk: INVALID_SET_FILE_POINTER Error");
		return hr;
	}

	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD bytesRead = 0;
	DWORD dwOffset = 0;

	while (hr == S_OK)
	{
		DWORD dwRead;
		if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL)) {
			hr = HRESULT_FROM_WIN32(GetLastError());
			ErrorLogger::Log(hr, "AudioEngine::FindChunk: ReadFile Error");
		}
		if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL)) {
			hr = HRESULT_FROM_WIN32(GetLastError());
			ErrorLogger::Log(hr, "AudioEngine::FindChunk: ReadFile Error");
		}
		switch (dwChunkType)
		{
		case fourccRIFF:
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL)) {
				hr = HRESULT_FROM_WIN32(GetLastError());
				ErrorLogger::Log(hr, "AudioEngine::FindChunk: ReadFile Error");
			}
			break;

		default:
			if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT)) {
				hr = HRESULT_FROM_WIN32(GetLastError());
				ErrorLogger::Log(hr, "AudioEngine::FindChunk: INVALID_SET_FILE_POINTER Error");
				return hr;
			}
		}

		dwOffset += sizeof(DWORD) * 2;

		if (dwChunkType == fourcc)
		{
			dwChunkSize = dwChunkDataSize;
			dwChunkDataPosition = dwOffset;
			return S_OK;
		}

		dwOffset += dwChunkDataSize;

		if (bytesRead >= dwRIFFDataSize) return S_FALSE;

	}

	return S_OK;
}

HRESULT AudioEngine::ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN)) {
		hr = HRESULT_FROM_WIN32(GetLastError());
		ErrorLogger::Log(hr, "AudioEngine::ReadChunkData: INVALID_SET_FILE_POINTER Error");
		return hr;
	}
	DWORD dwRead;
	if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL)) {
		hr = HRESULT_FROM_WIN32(GetLastError());
		ErrorLogger::Log(hr, "AudioEngine::ReadChunkData: ReadFile Error");
	}
	return hr;
}

void AudioEngine::DeleteAudioData(std::shared_ptr<SoundBankFile> soundBankFile)
{
	delete soundBankFile->buffer->pAudioData;
	delete soundBankFile->buffer->pContext;
	delete soundBankFile->buffer;
	soundBankFile->buffer = nullptr;
	delete soundBankFile->sourceFormat;
	soundBankFile->sourceFormat = nullptr;
	soundBankFile = nullptr;
}

std::shared_ptr<SoundBankFile> AudioEngine::CreateSoundBankFile(std::wstring filePath, std::string tagName, XAUDIO2_BUFFER* buffer, WAVEFORMATEX* waveformatex, float volume, bool randomPitchEnabled, float pitchMinimum, float pitchMaximum)
{
	std::shared_ptr<SoundBankFile> soundBankFile = std::make_shared<SoundBankFile>();
	soundBankFile->fileName = GetFileName(filePath);
	soundBankFile->tagName = tagName;
	soundBankFile->buffer = buffer;
	soundBankFile->sourceFormat = waveformatex;
	soundBankFile->volume = volume;
	soundBankFile->randomPitch = randomPitchEnabled;
	soundBankFile->pitchMin = pitchMinimum;
	soundBankFile->pitchMax = pitchMaximum;

	return soundBankFile;
}

void AudioEngine::AddToSoundBank(std::shared_ptr<SoundBankFile> soundBankFile, std::vector<std::shared_ptr<SoundBankFile>>& soundBank)
{
	soundBankFile->soundBankAddress = &soundBank;
	soundBank.push_back(soundBankFile);
}

void AudioEngine::CheckSoundBankExistence(std::string soundBankName, AudioType audioType)
{
	switch (audioType)
	{
	case SFX:
		if (m_SFXSoundBankMap.find(soundBankName) == m_SFXSoundBankMap.end()) {
			std::vector<std::shared_ptr<SoundBankFile>> newVector;
			m_SFXSoundBankMap.insert(std::pair<std::string, std::vector<std::shared_ptr<SoundBankFile>>>(soundBankName, newVector));
		}
		break;
	case MUSIC:
		if (m_MusicSoundBankMap.find(soundBankName) == m_MusicSoundBankMap.end()) {
			std::vector<std::shared_ptr<SoundBankFile>> newVector;
			m_MusicSoundBankMap.insert(std::pair<std::string, std::vector<std::shared_ptr<SoundBankFile>>>(soundBankName, newVector));
		}
		break;
	default:
		break;
	}
}

std::wstring AudioEngine::GetFileName(std::wstring filePath)
{
	std::filesystem::path fileName(filePath);

	return fileName.stem();
}

std::wstring AudioEngine::GetFileName(std::string filePath)
{
	std::filesystem::path fileName(StringHelper::StringToWide(filePath));

	return fileName.stem();
}

std::string AudioEngine::GetFileNameString(std::wstring filePath)
{
	std::filesystem::path fileName(filePath);
	std::string fileNameString = StringHelper::StringToNarrow(fileName.stem());

	return fileNameString;
}

std::string AudioEngine::GetFileNameString(std::string filePath)
{
	std::filesystem::path fileName(StringHelper::StringToWide(filePath));
	std::string fileNameString = StringHelper::StringToNarrow(fileName.stem());

	return fileNameString;
}

std::vector<std::shared_ptr<SoundBankFile>>& AudioEngine::GetSoundBank(std::string soundBankName, AudioType audioType) {
	switch (audioType)
	{
	case SFX:
		for (const auto& [key, value] : m_SFXSoundBankMap) {
			if (key == soundBankName) {
				return m_SFXSoundBankMap[key];
			}
		}
		break;
	case MUSIC:
		for (const auto& [key, value] : m_MusicSoundBankMap) {
			if (key == soundBankName) {
				return m_MusicSoundBankMap[key];
			}
		}
		break;
	default:
		ErrorLogger::Log("AudioEngine::GetSoundBank: Invalid audio type");
		break;
	}
}

std::shared_ptr<SoundBankFile> AudioEngine::FindSoundBankFile(std::wstring fileName, std::string soundBankName, AudioType audioType)
{
	std::vector<std::shared_ptr<SoundBankFile>>& soundBank = GetSoundBank(soundBankName, audioType);
	for (int i = 0; GetSoundBank(soundBankName, audioType).size() > i; i++) {
		if (fileName == GetSoundBank(soundBankName, audioType).at(i)->fileName) {
			return GetSoundBank(soundBankName, audioType).at(i);
		}
	}

	ErrorLogger::Log("AudioEngine::FindSoundBankFile: Sound Bank File not found");
	return nullptr;
}

