#include "SoundDevice.h"

SoundDevice::SoundDevice() {
    if (FAILED(XAudio2Create(pXAudio2.put(), 0, XAUDIO2_DEFAULT_PROCESSOR))) {
        SysError(L"Failed to create IXAudio2");
    }

#ifndef NDEBUG
    XAUDIO2_DEBUG_CONFIGURATION dbc = {
        .TraceMask = XAUDIO2_LOG_ERRORS | XAUDIO2_LOG_WARNINGS,
        .LogFunctionName = true
    };
    pXAudio2->SetDebugConfiguration(&dbc);
#endif // !NDEBUG

    IXAudio2MasteringVoice* pMasterVoice;
    if (FAILED(pXAudio2->CreateMasteringVoice(&pMasterVoice))) {
        SysError(NULL);
    }

    XAUDIO2_VOICE_DETAILS vd;
    pMasterVoice->GetVoiceDetails(&vd);

    WAVEFORMATEX wfx = {
            .wFormatTag = WAVE_FORMAT_PCM,
            .nChannels = 1,
            .nSamplesPerSec = 44100,
            .nAvgBytesPerSec = 88200,
            .nBlockAlign = 2,
            .wBitsPerSample = 16,
            .cbSize = 0,
    };

    HRESULT hr;

    for (unsigned int i = 0; i < SOUND_SOURCE_COUNT; i++) {

        if (FAILED(hr = pXAudio2->CreateSourceVoice(&source_voice_pool[i], &wfx))) {
            SysError(L"Fail to create sound source");
        }
        source_voice_pool[i]->Start(0);
    }
}

void SoundDevice::play_once(const Sound& sound) const {
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
