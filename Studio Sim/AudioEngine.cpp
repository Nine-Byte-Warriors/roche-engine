#include "stdafx.h"
#include "AudioEngine.h"



AudioEngine* AudioEngine::m_pAudioEngineInstance{ nullptr };
std::mutex AudioEngine::m_mutex;

AudioEngine::AudioEngine() : m_pXAudio2(nullptr), m_pMasterVoice(nullptr) {

}

AudioEngine* AudioEngine::GetInstance()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	if (m_pAudioEngineInstance == nullptr) {
		m_pAudioEngineInstance = new AudioEngine();
	}
	return m_pAudioEngineInstance;
}

void AudioEngine::Initialize(int numberOfSFXSourceVoices, int numberOfMusicSourceVoices)
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
	
	// Create source voice list 
	for (int i = 0; m_vSFXSourceVoices.size() < i; i++) {
		if (FAILED(hr = m_pXAudio2->CreateSourceVoice(&m_vSFXSourceVoices[i]->sourceVoice))) {
			ErrorLogger::Log(hr, "Failed to CreateSourceVoice");
			return hr;
		}
	}

	// Remove Later
	//ParseAudio(L"TestAudioFiles\\pcm-32bit-44khz-mono.wav", pSourceVoice2);

	ParseAudio(L"TestAudioFiles\\piano2.wav", pSourceVoice);
	ParseAudio(L"TestAudioFiles\\quietlaugh.wav", pSourceVoice2);



}

void AudioEngine::Update(float deltaTime)
{
	
}

HRESULT AudioEngine::LoadAudio(std::wstring filePath)
{
	HRESULT hr = S_OK;

	//// Create structures
	WAVEFORMATEXTENSIBLE wfx = { 0 };
	XAUDIO2_BUFFER buffer = { 0 };



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
		ErrorLogger::Log(hr, "AudioEngine::ParseAudio: INVALID_HANDLE_VALUE Error");
		return hr;
	}

	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN)) {
		hr = HRESULT_FROM_WIN32(GetLastError());
		ErrorLogger::Log(hr, "AudioEngine::ParseAudio: INVALID_SET_FILE_POINTER Error");
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
		ErrorLogger::Log(hr, "AudioEngine::ParseAudio: File type is not WAVE");
		return hr;
	}

	FindChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
	ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);

	//fill out the audio data buffer with the contents of the fourccDATA chunk
	FindChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
	BYTE* pDataBuffer = new BYTE[dwChunkSize];
	ReadChunkData(hFile, pDataBuffer, dwChunkSize, dwChunkPosition);

	buffer.AudioBytes = dwChunkSize;  //size of the audio buffer in bytes
	buffer.pAudioData = pDataBuffer;  //buffer containing audio data
	buffer.Flags = XAUDIO2_END_OF_STREAM; // tell the source voice not to expect any data after this buffer
}

HRESULT AudioEngine::PlayAudio(std::wstring fileName)
{
	HRESULT hr = S_OK;

	// Check with the file name exists, if so, grab a buffer from it
	for (int i = 0; m_vSFXSoundBank.size() < i; i++) {
		if (fileName == m_vSFXSoundBank[i]->fileName) {
			// Check whether the sound can be allocated into any source voice
			// Basically if there is source voice without a buffer or finds the lower priority sound
			// Replaces THE LOWEST priority sound
			for (int j = 0; m_vSFXSourceVoices.size() < j; j++) {
				
			}






		}
	}


	//if (FAILED(hr = sourceVoice->SubmitSourceBuffer(&buffer))) {
	//	ErrorLogger::Log(hr, "Failed to SubmitSourceBuffer");
	//	return hr;
	//}

	//if (FAILED(hr = sourceVoice->Start(0))) {
	//	ErrorLogger::Log(hr, "Failed to start Source Voice");
	//	return hr;
	//}

	return hr;
}

HRESULT AudioEngine::ParseAudio(std::wstring filePath, IXAudio2SourceVoice* sourceVoice)
{
	HRESULT hr = S_OK;

	//// Create structures
	WAVEFORMATEXTENSIBLE wfx = { 0 };
	XAUDIO2_BUFFER buffer = { 0 };

	

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
		ErrorLogger::Log(hr, "AudioEngine::ParseAudio: INVALID_HANDLE_VALUE Error");
		return hr;
	}

	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN)) {
		hr = HRESULT_FROM_WIN32(GetLastError());
		ErrorLogger::Log(hr, "AudioEngine::ParseAudio: INVALID_SET_FILE_POINTER Error");
		return hr;
	}

	// Find RIFF chunk and check the file type
	DWORD dwChunkSize;
	DWORD dwChunkPosition;
	//check the file type, should be fourccWAVE or 'XWMA'
	FindChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
	DWORD filetype;
	ReadChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);

	if (filetype != fourccWAVE){
		hr = S_FALSE;
		ErrorLogger::Log(hr, "AudioEngine::ParseAudio: File type is not WAVE");
		return hr;
	}

	FindChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
	ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);

	//fill out the audio data buffer with the contents of the fourccDATA chunk
	FindChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
	BYTE* pDataBuffer = new BYTE[dwChunkSize];
	ReadChunkData(hFile, pDataBuffer, dwChunkSize, dwChunkPosition);

	buffer.AudioBytes = dwChunkSize;  //size of the audio buffer in bytes
	buffer.pAudioData = pDataBuffer;  //buffer containing audio data
	buffer.Flags = XAUDIO2_END_OF_STREAM; // tell the source voice not to expect any data after this buffer

	// Put the buffer into container here
	//
	//vector?
	//
	//

	// TEST


	if (FAILED(hr = m_pXAudio2->CreateSourceVoice(&sourceVoice, (WAVEFORMATEX*)&wfx))) {
		ErrorLogger::Log(hr, "Failed to CreateSourceVoice");
		return hr;
	}

	if (FAILED(hr = sourceVoice->SubmitSourceBuffer(&buffer))) {
		ErrorLogger::Log(hr, "Failed to SubmitSourceBuffer");
		return hr;
	}

	if (FAILED(hr = sourceVoice->Start(0))) {
		ErrorLogger::Log(hr, "Failed to start Source Voice");
		return hr;
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

