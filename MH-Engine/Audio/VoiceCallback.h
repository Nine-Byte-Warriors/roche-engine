#pragma once
#ifndef VOICECALLBACK_H
#define VOICECALLBACK_H

//#include "AudioEngine.h"
#include <xaudio2.h>

//#include <thread>
//#include <mutex>

class VoiceCallback : public IXAudio2VoiceCallback
{
public:
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

#endif //AUDIOENGINE_H