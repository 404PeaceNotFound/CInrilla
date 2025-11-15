#include "audio.h"

static Sound uiSound;
static Sound menuMusic;

void InitAudioSystem(void) {
    InitAudioDevice();
    
    uiSound = LoadSound("sounds/UI_SOUND.mp3");
    menuMusic = LoadSound("sounds/Sound_Menu.mp3");
    
    SetSoundVolume(uiSound, 10);
    SetSoundVolume(menuMusic, 0.25f);
    
    PlaySound(menuMusic);
}

void PlayUISound(void) {
    PlaySound(uiSound);
}

void PlayMenuMusic(void) {
    PlaySound(menuMusic);
}

void CloseAudioSystem(void) {
    UnloadSound(uiSound);
    UnloadSound(menuMusic);
    CloseAudioDevice();
}