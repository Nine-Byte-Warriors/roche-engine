#include "stdafx.h"
#include "AudioEngine.h"

std::shared_ptr<AudioEngine> AudioEngine::m_pAudioEngineInstance{ nullptr };
std::mutex AudioEngine::m_mutex;

AudioEngine::AudioEngine() : m_pXAudio2(nullptr), m_pMasterVoice(nullptr), m_vSFXSoundBank(nullptr), m_vMusicSourceVoiceList(nullptr), m_vSFXSourceVoiceList(nullptr) {
	// Create sound banks
	//m_vMusicSoundBank = new std::vector<SoundBankFile*>();
	m_vMusicSoundBank = std::make_shared<std::vector<std::shared_ptr<SoundBankFile>>>();
	//m_vSFXSoundBank = new std::vector<SoundBankFile*>();
	m_vSFXSoundBank = std::make_shared<std::vector<std::shared_ptr<SoundBankFile>>>();

	// Create sound lists
	m_vMusicSourceVoiceList = std::make_shared<std::vector<std::shared_ptr<IXAudio2SourceVoice>>>();
	m_vSFXSourceVoiceList = std::make_shared<std::vector<std::shared_ptr<IXAudio2SourceVoice>>>();
}

AudioEngine::~AudioEngine() { }

AudioEngine* AudioEngine::GetInstance()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	if (m_pAudioEngineInstance == nullptr) {
		m_pAudioEngineInstance = std::make_shared<AudioEngine>();
	}

	return m_pAudioEngineInstance.get();
}

void AudioEngine::Initialize(float masterVolume, float musicVolume, float sfxVolume, int maxMusicSourceVoices, int maxSFXSourceVoices)
{
	HRESULT hr;
	//IXAUDIO2 arrayx 
	//std::shared_ptr<std::shared_ptr<IXAudio2>> testx = std::make_shared<std::shared_ptr<IXAudio2>>(m_pXAudio2);
	
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

	//LoadAudioFromJSON("Resources\\Audio\\Sound Banks\\soundFileList.json");
	//SaveAudioToJSON(m_vSFXSoundBank, m_vMusicSoundBank, "soundFiles");

}

void AudioEngine::Update()
{
	XAUDIO2_VOICE_STATE state;

	for (int i = 0; m_vSFXSourceVoiceList->size() > i; i++) {
		if (m_vSFXSourceVoiceList->at(i)->GetState(&state), state.BuffersQueued <= 0)
		{
			m_vSFXSourceVoiceList->at(i)->DestroyVoice();
			m_vSFXSourceVoiceList->erase(m_vSFXSourceVoiceList->begin() + i	);
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
		LoadAudio(StringHelper::StringToWide(soundFileListToLoad.at(i).filePath), soundFileListToLoad.at(i).volume, (AudioType)soundFileListToLoad.at(i).audioType);
	}
}

void AudioEngine::SaveAudioToJSON(std::shared_ptr<std::vector<std::shared_ptr<SoundBankFile>>> sfxSoundList, std::shared_ptr<std::vector<std::shared_ptr<SoundBankFile>>> musicSoundList, std::string fileName)
{
	std::vector<JSONSoundFile> soundFileList;
	std::string name;

	for (int i = 0; sfxSoundList->size() > i; i++) {
		name = StringHelper::StringToNarrow(sfxSoundList->at(i)->fileName);
		soundFileList.push_back({ name, "Resources\\Audio\\" + name + ".wav", sfxSoundList->at(i)->volume, 0 });
	}

	for (int i = 0; musicSoundList->size() > i; i++) {
		name = StringHelper::StringToNarrow(musicSoundList->at(i)->fileName);
		soundFileList.push_back({ name, "Resources\\Audio\\" + name + ".wav", musicSoundList->at(i)->volume, 1 });
	}

	JsonLoading::SaveJson(soundFileList, "Resources\\Audio\\Sound Banks\\" + fileName + ".json");
}

HRESULT AudioEngine::LoadAudio(std::wstring filePath, float volume, AudioType audioType)
{
	HRESULT hr = S_OK;

	//// Create structures
	std::shared_ptr<WAVEFORMATEXTENSIBLE> wfx = std::make_shared<WAVEFORMATEXTENSIBLE>();
	std::shared_ptr<XAUDIO2_BUFFER> buffer = std::make_shared<XAUDIO2_BUFFER>();

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
	std::shared_ptr ft = std::make_shared<DWORD>(&filetype); /// WARNING ?!!??!?!?
	ReadChunkData(hFile, ft, sizeof(DWORD), dwChunkPosition);

	if (filetype != fourccWAVE) {
		hr = S_FALSE;
		ErrorLogger::Log(hr, "AudioEngine::LoadAudio: File type is not WAVE");
		return hr;
	}

	FindChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
	ReadChunkData(hFile, wfx, dwChunkSize, dwChunkPosition);

	//fill out the audio data buffer with the contents of the fourccDATA chunk
	FindChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
	std::shared_ptr<BYTE> pDataBuffer = std::make_shared<BYTE>(dwChunkSize);
	ReadChunkData(hFile, pDataBuffer, dwChunkSize, dwChunkPosition);

	buffer->AudioBytes = dwChunkSize;  // size of the audio buffer in bytes
	buffer->pAudioData = pDataBuffer.get();  // buffer containing audio data
	buffer->Flags = XAUDIO2_END_OF_STREAM; // tell the source voice not to expect any data after this buffer

	//std::shared_ptr<WAVEFORMATEX> test = std::reinterpret_pointer_cast<WAVEFORMATEXTENSIBLE>(wfx);
	//WAVEFORMATEXTENSIBLE* wew = new WAVEFORMATEXTENSIBLE();
	std::shared_ptr<WAVEFORMATEX> waveformatexPtr = std::make_shared<WAVEFORMATEX>(wfx->Format);

	AddToSoundBank(CreateSoundBankFile(filePath, buffer, waveformatexPtr, volume), GetSoundBank(audioType));

	return hr;
}

HRESULT AudioEngine::PlayAudio(std::wstring fileName, AudioType audioType)
{
	HRESULT hr = S_OK;
	float finalVolume;

	if (audioType == SFX && m_vSFXSourceVoiceList->size() >= m_iMaxSFXSourceVoicesLimit) {
		return S_FALSE;
	}
	else if (audioType == MUSIC && m_vMusicSourceVoiceList->size() >= m_iMaxMusicSourceVoicesLimit) {
		return S_FALSE;
	}

	std::shared_ptr<IXAudio2SourceVoice> pVoice = nullptr;
	//std::shared_ptr<IXAudio2SourceVoice> pVoice = nullptr;
	//Microsoft::WRL::ComPtr<IXAudio2SourceVoice> pVoice = nullptr;
	std::shared_ptr<std::vector<std::shared_ptr<SoundBankFile>>> soundBank = GetSoundBank(audioType);

	// Check with the file name exists, if so, grab a buffer from it
	for (int i = 0; soundBank->size() > i; i++) {
		if (fileName == soundBank->at(i)->fileName) {

			if (FAILED(hr = m_pXAudio2->CreateSourceVoice(&pVoice.get(), soundBank->at(i)->sourceFormat.get(), 0, XAUDIO2_DEFAULT_FREQ_RATIO,
				NULL/*soundBank->at(i)->voiceCallback*/, NULL, NULL))) {
				ErrorLogger::Log(hr, "AudioEngine::PlayAudio: Failed to CreateSourceVoice");
				return hr;
			}

			if (FAILED(hr = pVoice->SubmitSourceBuffer(soundBank->at(i)->buffer.get()))) {
				ErrorLogger::Log(hr, "AudioEngine::PlayAudio: Failed to SubmitSourceBuffer");
				return hr;
			}

			switch (audioType)
			{
			case SFX:
				finalVolume = m_fMasterVolume * m_fSFXVolume * soundBank->at(i)->volume;
				pVoice->SetVolume(finalVolume);
				m_vSFXSourceVoiceList->push_back(std::move(pVoice));
				break;
			case MUSIC:
				finalVolume = m_fMasterVolume * m_fMusicVolume * soundBank->at(i)->volume;
				pVoice->SetVolume(finalVolume);
				m_vMusicSourceVoiceList->push_back(std::move(pVoice));
				break;
			default:
				ErrorLogger::Log("AudioEngine::PlayAudio: Failed to add source voice to the list");
				break;
			}

			if (FAILED(hr = pVoice->Start(0))) {
				ErrorLogger::Log(hr, "AudioEngine::PlayAudio: Failed to start Source Voice");
				return hr;
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

		m_bIsMusicPaused = false;
	}

	return hr;
}

HRESULT AudioEngine::PauseMusic()
{
	HRESULT hr = S_OK;

	for (int i = 0; m_vMusicSourceVoiceList->size() > i; i++) {
		if (FAILED(hr = m_vMusicSourceVoiceList->at(i)->Stop(0))) {
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

	for (int i = 0; m_vMusicSourceVoiceList->size() > i; i++) {
		if (FAILED(hr = m_vMusicSourceVoiceList->at(i)->Stop(0))) {
			ErrorLogger::Log(hr, "AudioEngine::StopMusic: Failed to Stop music");
			return hr;
		}
		else {
			m_vMusicSourceVoiceList->at(i)->DestroyVoice();
			m_vMusicSourceVoiceList->erase(m_vMusicSourceVoiceList->begin() + i);
		}
	}

	return hr;
}

HRESULT AudioEngine::UnloadAudio(std::wstring fileName, AudioType audioType)
{
	HRESULT hr = S_OK;

	std::shared_ptr<std::vector<std::shared_ptr<SoundBankFile>>> soundBank = GetSoundBank(audioType);

	for (int i = 0; soundBank->size() > i; i++) {
		if (fileName == soundBank->at(i)->fileName) {
			// change this later on change from raw pointers to smart pointers
			delete soundBank->at(i)->buffer->pAudioData;
			delete soundBank->at(i)->buffer->pContext;
			//delete soundBank->at(i)->buffer;
			soundBank->at(i)->buffer = nullptr;
			//delete soundBank->at(i)->sourceFormat;
			soundBank->at(i)->sourceFormat = nullptr;
			//delete soundBank->at(i);
			soundBank->at(i) = nullptr;
			soundBank->erase(soundBank->begin() + i);

			return hr;
		}
	}

	return hr;
}

void AudioEngine::StopAllAudio()
{
	for (int i = 0; m_vSFXSourceVoiceList->size() > i; i++) {
		m_vSFXSourceVoiceList->at(i)->DestroyVoice();
		m_vSFXSourceVoiceList->erase(m_vSFXSourceVoiceList->begin() + i);
	}

	for (int i = 0; m_vMusicSourceVoiceList->size() > i; i++) {
		m_vMusicSourceVoiceList->at(i)->DestroyVoice();
		m_vMusicSourceVoiceList->erase(m_vMusicSourceVoiceList->begin() + i);
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

HRESULT AudioEngine::ReadChunkData(HANDLE hFile, std::shared_ptr<void> buffer, DWORD buffersize, DWORD bufferoffset)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN)) {
		hr = HRESULT_FROM_WIN32(GetLastError());
		ErrorLogger::Log(hr, "AudioEngine::ReadChunkData: INVALID_SET_FILE_POINTER Error");
		return hr;
	}
	DWORD dwRead;
	if (0 == ReadFile(hFile, &buffer, buffersize, &dwRead, NULL)) { // WARNING
		hr = HRESULT_FROM_WIN32(GetLastError());
		ErrorLogger::Log(hr, "AudioEngine::ReadChunkData: ReadFile Error");
	}
	return hr;
}

std::shared_ptr<SoundBankFile> AudioEngine::CreateSoundBankFile(std::wstring filePath, std::shared_ptr<XAUDIO2_BUFFER> buffer, std::shared_ptr<WAVEFORMATEX> waveformatex, float volume)
{
	std::shared_ptr<SoundBankFile> soundBankFile = std::make_shared<SoundBankFile>();
	soundBankFile->fileName = GetFileName(filePath);
	soundBankFile->buffer = buffer;
	soundBankFile->sourceFormat = waveformatex;
	//soundBankFile->voiceCallback = new VoiceCallback();
	//soundBankFile->voiceCallback->OnBufferEnd(buffer);
	soundBankFile->volume = volume;

	return soundBankFile;
}

void AudioEngine::AddToSoundBank(std::shared_ptr<SoundBankFile> soundBankFile, std::shared_ptr<std::vector<std::shared_ptr<SoundBankFile>>> soundBank)
{
	// Optionally TODO:
	// Check whether the same file name in SoundBankFile is being used in other SoundBankFile, if so resolve it
	soundBank->push_back(std::move(soundBankFile));
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

std::shared_ptr<std::vector<std::shared_ptr<SoundBankFile>>> AudioEngine::GetSoundBank(AudioType audioType) {
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

SoundBankFile* AudioEngine::FindSoundBankFile(std::wstring fileName, AudioType audioType)
{
	for (int i = 0; GetSoundBank(audioType)->size() > i; i++) {
		if (fileName == GetSoundBank(audioType)->at(i)->fileName) {
			return GetSoundBank(audioType)->at(i).get();
		}
	}

	ErrorLogger::Log("AudioEngine::FindSoundBankFile: Sound Bank File not found");
	return nullptr;
}
