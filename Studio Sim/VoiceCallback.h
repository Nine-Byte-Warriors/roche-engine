#pragma once
#include <xaudio2.h>
//#include "AudioEngine.h"

#include <thread>
#include <mutex>
#include "stdafx.h"

class VoiceCallback : public IXAudio2VoiceCallback
{
public:
    //AudioEngine* audioEnginePtr = AudioEngine::GetInstance();

    HANDLE hBufferEndEvent;

    VoiceCallback();
    ~VoiceCallback();

    //Called when the voice has just finished playing a contiguous audio stream.
    void OnStreamEnd();

    //Unused methods are stubs
    void OnVoiceProcessingPassEnd() { }
    void OnVoiceProcessingPassStart(UINT32 SamplesRequired) {    }
    void OnBufferEnd(void* pBufferContext);
    void OnBufferStart(void* pBufferContext) {   }
    void OnLoopEnd(void* pBufferContext) {    }
    void OnVoiceError(void* pBufferContext, HRESULT Error) { }
};