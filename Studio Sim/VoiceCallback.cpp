#include "stdafx.h"
#include "VoiceCallback.h"

VoiceCallback::VoiceCallback() : hBufferEndEvent(CreateEvent(NULL, FALSE, FALSE, NULL)) {
}

VoiceCallback::~VoiceCallback() {
	CloseHandle(hBufferEndEvent);
}

void VoiceCallback::OnStreamEnd() {
	SetEvent(hBufferEndEvent);
}

void VoiceCallback::OnBufferEnd(void* pBufferContext) {

	// Set event
	// remove from counter

}