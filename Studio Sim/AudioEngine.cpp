#include "stdafx.h"
#include "AudioEngine.h"

AudioEngine* AudioEngine::m_pAudioEngineInstance{ nullptr };
std::mutex AudioEngine::m_mutex;

AudioEngine::AudioEngine() : m_pXAudio2(nullptr), m_pMasterVoice(nullptr), m_vSFXSoundBank(nullptr), m_vMusicSourceVoiceList(nullptr), m_vSFXSourceVoiceList(nullptr) {
	// Create sound banks
	m_vMusicSoundBank = new std::vector<SoundBankFile*>();
	m_vSFXSoundBank = new std::vector<SoundBankFile*>();

	// Create sound lists
	m_vMusicSourceVoiceList = new std::vector<IXAudio2SourceVoice*>;
	m_vSFXSourceVoiceList = new std::vector<IXAudio2SourceVoice*>;
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

	//m_iSFXSourceVoicesPlaying = 0;
	//m_iMusicSourceVoicesPlaying = 0;

	// Load Sound Bank for SFX
	// Handle it with JSON later
	// Current one is the temporary implementation
	LoadAudio(L"Resources\\AudioFiles\\pcm-32bit-44khz-mono.wav", 1.0f, SFX);
	LoadAudio(L"Resources\\AudioFiles\\pcm-32bit-44khz-stereo.wav", 1.0f, SFX);
	LoadAudio(L"Resources\\AudioFiles\\piano2.wav", 1.0f, SFX);
	LoadAudio(L"Resources\\AudioFiles\\quietlaugh.wav", 1.0f, SFX);
	LoadAudio(L"Resources\\AudioFiles\\bullettest.wav", 1.0f, SFX);

	// Load Sound Bank for Music
	// Handle it with JSON later
	// Current one is the temporary implementation
	LoadAudio(L"Resources\\AudioFiles\\creepymusic.wav", 1.0f, MUSIC);
	LoadAudio(L"Resources\\AudioFiles\\partymusic.wav", 1.0f, MUSIC);

}

void AudioEngine::Update()
{
	XAUDIO2_VOICE_STATE state;

	for (int i = 0; m_vSFXSourceVoiceList->size() > i; i++) {
		if (m_vSFXSourceVoiceList->at(i)->GetState(&state), state.BuffersQueued <= 0)
		{
			m_vSFXSourceVoiceList->erase(m_vSFXSourceVoiceList->begin() + i	);
		}
	}

}

HRESULT AudioEngine::LoadAudio(std::wstring filePath, float volume, AudioType audioType)
{
	HRESULT hr = S_OK;

	//// Create structures
	WAVEFORMATEXTENSIBLE* wfx = new WAVEFORMATEXTENSIBLE();
	XAUDIO2_BUFFER* buffer = new XAUDIO2_BUFFER();


	//// Open audio with CreateFile
	//WCHAR* strFileName = (WCHAR*)TEXT("TestAudioFiles\\pcm-32bit-44khz-mono.wav");
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

	AddToSoundBank(CreateSoundBankFile(filePath, buffer, (WAVEFORMATEX*)wfx, volume), GetSoundBank(audioType));

	return hr;
}

HRESULT AudioEngine::PlayAudio(std::wstring fileName, AudioType audioType)
{
	HRESULT hr = S_OK;

	if (audioType == SFX && m_vSFXSourceVoiceList->size() >= m_iMaxSFXSourceVoicesLimit) {
		return S_FALSE;
	}
	else if (audioType == MUSIC && m_vMusicSourceVoiceList->size() >= m_iMaxMusicSourceVoicesLimit) {
		return S_FALSE;
	}

	IXAudio2SourceVoice* pVoice = nullptr;

	std::vector<SoundBankFile*>* soundBank = GetSoundBank(audioType);


		// Check with the file name exists, if so, grab a buffer from it
	for (int i = 0; soundBank->size() > i; i++) {
		if (fileName == soundBank->at(i)->fileName) {

			if (FAILED(hr = m_pXAudio2->CreateSourceVoice(&pVoice, soundBank->at(i)->sourceFormat, 0, XAUDIO2_DEFAULT_FREQ_RATIO,
				NULL/*soundBank->at(i)->voiceCallback*/, NULL, NULL))) {
				ErrorLogger::Log(hr, "AudioEngine::PlayAudio: Failed to CreateSourceVoice");
				return hr;
			}

			if (FAILED(hr = pVoice->SubmitSourceBuffer(soundBank->at(i)->buffer))) {
				ErrorLogger::Log(hr, "AudioEngine::PlayAudio: Failed to SubmitSourceBuffer");
				return hr;
			}

			if (FAILED(hr = pVoice->Start(0))) {
				ErrorLogger::Log(hr, "AudioEngine::PlayAudio: Failed to start Source Voice");
				return hr;
			}

			switch (audioType)
			{
			case SFX:
				pVoice->SetVolume(m_fMasterVolume * m_fSFXVolume * soundBank->at(i)->volume);
				m_vSFXSourceVoiceList->push_back(pVoice);
				break;
			case MUSIC:
				pVoice->SetVolume(m_fMasterVolume * m_fMusicVolume * soundBank->at(i)->volume);
				m_vMusicSourceVoiceList->push_back(pVoice);
				break;
			default:
				ErrorLogger::Log("AudioEngine::PlayAudio: Failed to add source voice to the list");
				break;
			}
			
			//pVoice->SetFrequencyRatio(12000);
			//
			//WaitForSingleObjectEx(m_vSFXSoundBank->at(i)->voiceCallback->hBufferEndEvent, INFINITE, TRUE);


		}
	} 

	return hr;
}

HRESULT AudioEngine::UnpauseMusic()
{
	HRESULT hr = S_OK;
	
	if (m_bIsMusicPaused) {
		for (int i = 0; m_vMusicSourceVoiceList->size() > i; i++) {
			m_vMusicSourceVoiceList->at(i)->Start(0);
		}

		m_bIsMusicPaused == false;
	}

	return hr;
}

HRESULT AudioEngine::PauseMusic()
{
	HRESULT hr = S_OK;

	for (int i = 0; m_vMusicSourceVoiceList->size() > i; i++) {
		if (FAILED(hr = m_vMusicSourceVoiceList->at(i)->Stop())) {
			ErrorLogger::Log(hr, "AudioEngine::PauseMusic: Failed to Stop music");
			return hr;
		}
	}

	m_bIsMusicPaused == true;

	return hr;
}

HRESULT AudioEngine::StopMusic()
{
	HRESULT hr = S_OK;

	for (int i = 0; m_vMusicSourceVoiceList->size() > i; i++) {
		if (FAILED(hr = m_vMusicSourceVoiceList->at(i)->Stop())) {
			ErrorLogger::Log(hr, "AudioEngine::StopMusic: Failed to Stop music");
			return hr;
		}
		else {
			m_vMusicSourceVoiceList->at(i)->DestroyVoice();
			m_vMusicSourceVoiceList->erase(m_vSFXSourceVoiceList->begin() + i);
		}
	}

	return hr;
}

HRESULT AudioEngine::UnloadAudio(std::wstring fileName, AudioType audioType)
{
	HRESULT hr = S_OK;

	std::vector<SoundBankFile*>* soundBank = GetSoundBank(audioType);

	for (int i = 0; soundBank->size() > i; i++) {
		if (fileName == soundBank->at(i)->fileName) {
			// change this later on change from raw pointers to smart pointers
			delete soundBank->at(i)->buffer->pAudioData;
			delete soundBank->at(i)->buffer->pContext;
			delete soundBank->at(i)->buffer;
			soundBank->at(i)->buffer = nullptr;
			delete soundBank->at(i)->sourceFormat;
			soundBank->at(i)->sourceFormat = nullptr;
			delete soundBank->at(i);
			soundBank->at(i) = nullptr;
			soundBank->erase(soundBank->begin() + i);

			return hr;
		}
	}

	return hr;
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

SoundBankFile* AudioEngine::CreateSoundBankFile(std::wstring filePath, XAUDIO2_BUFFER* buffer, WAVEFORMATEX* waveformatex, float volume)
{
	SoundBankFile* soundBankFile = new SoundBankFile();
	soundBankFile->fileName = GetFileName(filePath);
	soundBankFile->buffer = buffer;
	soundBankFile->sourceFormat = waveformatex;
	//soundBankFile->voiceCallback = new VoiceCallback();
	//soundBankFile->voiceCallback->OnBufferEnd(buffer);
	soundBankFile->volume = volume;

	return soundBankFile;
}

void AudioEngine::AddToSoundBank(SoundBankFile* soundBankFile, std::vector<SoundBankFile*>* soundBank)
{
	// Optionally TODO:
	// Check whether the same file name in SoundBankFile is being used in other SoundBankFile, if so resolve it
	soundBank->push_back(soundBankFile);
}

std::wstring AudioEngine::GetFileName(std::wstring filePath)
{
	std::filesystem::path fileName(filePath);

	return fileName.stem();
}

std::vector<SoundBankFile*>* AudioEngine::GetSoundBank(AudioType audioType) {
	switch (audioType)
	{
	case SFX:
		return m_vSFXSoundBank;
		break;
	case MUSIC:
		return m_vMusicSoundBank;
		break;
	default:
		ErrorLogger::Log("AudioEngine::GetSoundBank: Invalid audio type");
		break;
	}
}

