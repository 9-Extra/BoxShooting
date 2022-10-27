#pragma once

#include <xaudio2.h>
#include <memory>
#include "Error.h"

class Sound
{
	friend class SoundDevice;
private:
	XAUDIO2_BUFFER buffer = { 0 };
	std::unique_ptr <BYTE[]> sound_buffer;
	IXAudio2SourceVoice* pSourceVoice;
public:
	void play_once() const {
		if (FAILED(pSourceVoice->SubmitSourceBuffer(&buffer))) {
			SysError(NULL);
		}
		pSourceVoice->Start(0);
	}

	void stop() const {
		pSourceVoice->Stop(0);
	}
};
