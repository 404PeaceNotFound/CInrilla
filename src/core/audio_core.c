#include <raylib.h>

// Variáveis estáticas internas
static Sound somInterface;
static Sound musicaMenu;

void InicializarSistemaAudio(void) {
    InitAudioDevice();
    somInterface = LoadSound("assets/sounds/UI_SOUND.mp3");
    musicaMenu = LoadSound("assets/sounds/Sound_Menu.mp3");
    
    SetSoundVolume(somInterface, 1.0f); // Volume normalizado (0.0 a 1.0)
    SetSoundVolume(musicaMenu, 0.25f);
    
    PlaySound(musicaMenu); // Toca música inicial
}

void TocarSomUI(void) {
    PlaySound(somInterface);
}

void EncerrarSistemaAudio(void) {
    UnloadSound(somInterface);
    UnloadSound(musicaMenu);
    CloseAudioDevice();
}