#include <WG/WG_Audio.h>

#define MAX_QUEUE_SIZE ((uint32_t)-1)

typedef struct _WG_AudioQueue{
    WAVEHDR header;
    void *data;
    uint32_t len;
    struct _WG_AudioQueue *next;
}WG_AudioQueue;


struct _WG_AudioDevice{
    HWAVEOUT hWaveOut;
    
    WAVEFORMATEX waveFormat;
    
    WG_AudioState audioState;
    
    WG_AudioQueue *audioQueue;
    uint32_t queue_size;

    HANDLE mutex;
    bool locked;
};


WG_AudioDevice* WG_OpenAudioDevice(WAVEFORMATEX waveFormat){

    WG_AudioDevice *audioDevice = malloc(sizeof(WG_AudioDevice));
    
    if(!audioDevice){
        return NULL;
    }

    audioDevice->waveFormat = waveFormat;

    MMRESULT result = waveOutOpen(
        &audioDevice->hWaveOut,
        WAVE_MAPPER,
        &audioDevice->waveFormat,
        0,
        0,
        CALLBACK_NULL
    );

    if(result != MMSYSERR_NOERROR){
        free(audioDevice);
        return NULL;
    }

    audioDevice->audioState = WG_AUDIO_STOPPED;

    audioDevice->audioQueue = NULL;
    audioDevice->queue_size = 0;

    audioDevice->mutex = CreateMutex(NULL,FALSE,NULL);

    if(!audioDevice->mutex){
        waveOutClose(audioDevice->hWaveOut);
        free(audioDevice);
        return NULL;
    }

    audioDevice->locked = false;

    return audioDevice;
}

void WG_CloseAudioDevice(WG_AudioDevice *audioDevice){
    WG_ClearQueueAudio(audioDevice);
    waveOutClose(audioDevice->hWaveOut);
    CloseHandle(audioDevice->mutex);
    free(audioDevice);
}

void WG_LockAudioDevice(WG_AudioDevice *audioDevice){

    HRESULT result = WaitForSingleObject(audioDevice->mutex,INFINITE);

    if(result != WAIT_OBJECT_0){
        if(result == WAIT_TIMEOUT){
            printf("Mutex took a long time to be released.\n");
        }
        else{
            printf("Error: failed to wait for mutex %d.\n",GetLastError());
        }
    }

    audioDevice->locked = true;
}

void WG_UnlockAudioDevice(WG_AudioDevice *audioDevice){

    if(audioDevice->locked == false) return;
    
    if(!ReleaseMutex(audioDevice->mutex)){
        printf("Error: failed to release mutex %d.\n",GetLastError());
    }

    audioDevice->locked = false;
}

void WG_PauseAudioDevice(WG_AudioDevice *audioDevice,bool pause){

    WG_UpdateQueueAudio(audioDevice);

    WG_LockAudioDevice(audioDevice);
    
    if(audioDevice->audioState != WG_AUDIO_STOPPED){
        
        if(pause == false && audioDevice->audioState == WG_AUDIO_PAUSED && waveOutRestart(audioDevice->hWaveOut) == MMSYSERR_NOERROR){
            
            audioDevice->audioState = WG_AUDIO_PLAYING;
        }
        else if(pause == true && audioDevice->audioState == WG_AUDIO_PLAYING && waveOutPause(audioDevice->hWaveOut) == MMSYSERR_NOERROR){
            
            audioDevice->audioState = WG_AUDIO_PAUSED;
        }
    }

    WG_UnlockAudioDevice(audioDevice);
}

WG_AudioState WG_GetAudioDeviceState(WG_AudioDevice *audioDevice){
    
    WG_UpdateQueueAudio(audioDevice);

    return audioDevice->audioState;
}

void WG_UpdateQueueAudio(WG_AudioDevice *audioDevice){
    
    WG_LockAudioDevice(audioDevice);

    WG_AudioQueue *audio = audioDevice->audioQueue;
    WG_AudioQueue *buffer = NULL;

    while(audio != NULL){
        
        buffer = audio->next;

        if(audio->header.dwFlags & WHDR_DONE){
            
            if(audio == audioDevice->audioQueue){
                audioDevice->audioQueue = buffer;
            }

            if(audio->header.dwFlags & WHDR_PREPARED){
                waveOutUnprepareHeader(audioDevice->hWaveOut,&audio->header,sizeof(WAVEHDR));
            }
            
            free(audio->data);
            free(audio);
        }

        audio = buffer;
    }

    if(!audioDevice->audioQueue){
        
        audioDevice->queue_size = 0;
        waveOutReset(audioDevice->hWaveOut);

        audioDevice->audioState = WG_AUDIO_STOPPED;
    }

    WG_UnlockAudioDevice(audioDevice);
}

void WG_QueueAudio(WG_AudioDevice *audioDevice,const void *data,uint32_t len){

    WG_UpdateQueueAudio(audioDevice);

    if(audioDevice->queue_size + len > MAX_QUEUE_SIZE) return;

    WG_AudioQueue *new = malloc(sizeof(WG_AudioQueue));
    
    new->data = malloc(len);
    new->len = len;
    new->header = (WAVEHDR){0};
    new->header.lpData = new->data;
    new->header.dwBufferLength = len;
    new->next = NULL;

    memcpy(new->data,data,len);

    WG_LockAudioDevice(audioDevice);

    if(!audioDevice->audioQueue){
        audioDevice->audioQueue = new;
    }
    else{
        WG_AudioQueue *buffer = audioDevice->audioQueue;
        while(buffer->next){buffer = buffer->next;}
        buffer->next = new;
    }

    audioDevice->queue_size += len;

    waveOutPrepareHeader(audioDevice->hWaveOut,&new->header,sizeof(WAVEHDR));
    waveOutWrite(audioDevice->hWaveOut,&new->header,sizeof(WAVEHDR));

    if(audioDevice->audioState == WG_AUDIO_STOPPED){

        audioDevice->audioState = WG_AUDIO_PLAYING;
    }

    WG_UnlockAudioDevice(audioDevice);
}

uint32_t WG_GetQueuedAudioSize(WG_AudioDevice *audioDevice){
    
    WG_UpdateQueueAudio(audioDevice);

    MMTIME time = {0};
    time.wType = TIME_BYTES;
    waveOutGetPosition(audioDevice->hWaveOut,&time,sizeof(MMTIME));

    return audioDevice->queue_size - time.u.cb; 
}

void WG_ClearQueueAudio(WG_AudioDevice *audioDevice){
    waveOutReset(audioDevice->hWaveOut);
    WG_UpdateQueueAudio(audioDevice);
}
