#pragma once
#include <xaudio2.h>
#include <winrt/base.h>
#include "Error.h"
#include "Sound.h"

struct SoundHolder {
    XAUDIO2_BUFFER buffer = { 0 };
    std::unique_ptr <BYTE[]> sound_buffer;
};

class SoundDevice {
private:
    winrt::com_ptr<IXAudio2> pXAudio2;
    std::vector<SoundHolder> loaded_sounds;
public:

    SoundDevice() {
        if (FAILED(XAudio2Create(pXAudio2.put(), 0, XAUDIO2_DEFAULT_PROCESSOR))) {
            SysError(L"Failed to create IXAudio2");
        }
        IXAudio2MasteringVoice *pMasterVoice;
        if (FAILED(pXAudio2->CreateMasteringVoice(&pMasterVoice))) {
            SysError(NULL);
        }
    }

    void load_sound(LPCWSTR path, Sound& sound);
};