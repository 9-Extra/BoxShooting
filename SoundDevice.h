#pragma once
#include <xaudio2.h>
#include <winrt/base.h>
#include "Error.h"
#include "Sound.h"

#define SOUND_SOURCE_COUNT 32

class SoundDevice {
private:
    winrt::com_ptr<IXAudio2> pXAudio2;
    IXAudio2SourceVoice* source_voice_pool[SOUND_SOURCE_COUNT];
public:

    SoundDevice();

    void load_sound(LPCWSTR path, Sound& sound);

	void play_once(const Sound& sound) const {
		for (unsigned int i = 0; i < SOUND_SOURCE_COUNT; i++) {
			IXAudio2SourceVoice* pSourceVoice = source_voice_pool[i];
			XAUDIO2_VOICE_STATE vs;
			pSourceVoice->GetState(&vs, XAUDIO2_VOICE_NOSAMPLESPLAYED);
			if (vs.BuffersQueued == 0) {
				if (FAILED(pSourceVoice->SubmitSourceBuffer(&sound.buffer))) {
					SysError(L"没能上传声音");
				}
				goto SUCCESS;
			}
		}
		debug_log("Fail to play sound!!!");

	SUCCESS:
		return;
	}
};