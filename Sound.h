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
public:
	
};
