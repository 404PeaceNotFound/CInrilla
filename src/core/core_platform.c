#include "core_platform.h"
#include "../config/config.h"
#include <raylib.h>

static Sound somInterface;
static Music musicaMenu;

void Core_Init(void) {
    InitWindow(LARGURA_TELA, ALTURA_TELA, TITULO_JOGO);
    InitAudioDevice();
    SetTargetFPS(ALVO_FPS);

    // Carregamento defensivo (assume que assets existem ou trata erro)
    if (FileExists("assets/sounds/UI_SOUND.mp3"))
        somInterface = LoadSound("assets/sounds/UI_SOUND.mp3");
    
    if (FileExists("assets/sounds/Sound_Menu.mp3")) {
        musicaMenu = LoadMusicStream("assets/sounds/Sound_Menu.mp3");
        SetMusicVolume(musicaMenu, 0.25f);
        musicaMenu.looping = true;
    }
}

void Core_Close(void) {
    if (IsSoundPlaying(somInterface)) UnloadSound(somInterface);
    if (IsMusicValid(musicaMenu)) UnloadMusicStream(musicaMenu);
    CloseAudioDevice();
    CloseWindow();
}

bool Core_ShouldClose(void) {
    return WindowShouldClose();
}

double Core_GetTime(void) {
    return GetFrameTime();
}

void Core_PlayMusic(void) {
    if (IsMusicValid(musicaMenu))if(!IsMusicStreamPlaying(musicaMenu)) PlayMusicStream(musicaMenu);
}

void Core_PlaySoundUI(void) {
    if (IsSoundValid(somInterface)) PlaySound(somInterface);
}

void Core_UpdateMusic(void) {
    // Verifica se a música existe e está pronta antes de atualizar
    if (IsMusicValid(musicaMenu)) {
        UpdateMusicStream(musicaMenu);
    }
}