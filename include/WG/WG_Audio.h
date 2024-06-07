#ifndef WG_AUDIO_H_
#define WG_AUDIO_H_

#include <windows.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct _WG_AudioDevice WG_AudioDevice;

typedef enum _WG_AudioState{
    WG_AUDIO_STOPPED,
    WG_AUDIO_PLAYING,
    WG_AUDIO_PAUSED
}WG_AudioState;


WG_AudioDevice* WG_OpenAudioDevice(WAVEFORMATEX waveFormat);

void WG_CloseAudioDevice(WG_AudioDevice *audioDevice);

void WG_LockAudioDevice(WG_AudioDevice *audioDevice);

void WG_UnlockAudioDevice(WG_AudioDevice *audioDevice);

void WG_PauseAudioDevice(WG_AudioDevice *audioDevice,bool pause);

WG_AudioState WG_GetAudioDeviceState(WG_AudioDevice *audioDevice);

void WG_UpdateQueueAudio(WG_AudioDevice *audioDevice);

void WG_QueueAudio(WG_AudioDevice *audioDevice,const void *data,uint32_t len);

uint32_t WG_GetQueuedAudioSize(WG_AudioDevice *audioDevice);

void WG_ClearQueueAudio(WG_AudioDevice *audioDevice);

#endif