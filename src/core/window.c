#include "window.h"
#include <raylib.h>

void InicializarJanela(void) {
    InitWindow(LARGURA_TELA, ALTURA_TELA, "CInrilla");
    SetTargetFPS(60);
}

void EncerrarJanela(void) {
    CloseWindow();
}