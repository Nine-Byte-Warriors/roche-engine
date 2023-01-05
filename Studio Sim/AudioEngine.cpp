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

void AudioEngine::Initialize()
{
	HRESULT hr;

	// Creates instance of XAudio2 engine
	if (FAILED(hr = XAudio2Create(&m_pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR))) {
		ErrorLogger::Log(hr, "AudioEngine: Failed to create XAudio2 engine.");
	}

	// Master voice encapsulates and audio device, the final destination where all audio sources go
	if (FAILED(hr = m_pXAudio2->CreateMasteringVoice(&m_pMasterVoice))) {
		ErrorLogger::Log(hr, "AudioEngine: Failed to create mastering voice.");
	}

	

}

HRESULT AudioEngine::ParseAudio()
{
	//// Create structures
	WAVEFORMATEXTENSIBLE wfx = { 0 };
	XAUDIO2_BUFFER buffer = { 0 };

	//// Open audio with CreateFile
	WCHAR* strFileName = new WCHAR('a'); ///// take a look later
	// Open file
	HANDLE hFile = CreateFile(
		strFileName,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return HRESULT_FROM_WIN32(GetLastError());

	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());


	// Find RIFF chunk and check the file type
	DWORD dwChunkSize;
	DWORD dwChunkPosition;
	//check the file type, should be fourccWAVE or 'XWMA'
	FindChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
	DWORD filetype;
	ReadChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);
	if (filetype != fourccWAVE)
		return S_FALSE;

}

HRESULT AudioEngine::FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkData)
{
	return E_NOTIMPL;
}

HRESULT AudioEngine::ReadChunkData(HANDLE hFile, void* filetype, DWORD buffersize, DWORD bufferoffset)
{
	return E_NOTIMPL;
}

//void AudioEngine::HandleEvent(Event* event)
//{
//	switch (event->GetEventID()) {
//		case EVENTID::LoadAudioEvent: {
//			// Load Audio
//		}
//		break;
//		case EVENTID::PlayAudioEvent: {
//			// Play Audio
//		}
//		break;
//		case EVENTID::StopAudioEvent: {
//			// Stop Audio
//		}
//		break;
//		case EVENTID::UnloadAudioEvent: {
//			// Unload Audio
//		}
//		break;
//	}
//}
