#pragma once
#include <xaudio2.h>
#include <winrt/base.h>
#include "Error.h"

class Sound
{
	friend class SoundLoader;
	friend class SoundDevice;
private:
	XAUDIO2_BUFFER buffer = { 0 };
	std::unique_ptr <BYTE[]> sound_buffer;
public:

};

#define SOUND_SOURCE_COUNT 32

class SoundDevice {
private:
    winrt::com_ptr<IXAudio2> pXAudio2;
    IXAudio2SourceVoice* source_voice_pool[SOUND_SOURCE_COUNT];
public:

    SoundDevice();

    void play_once(const Sound& sound) const;
};