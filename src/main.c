#include "game_logic.h"

int main(void) {
    InitGame();
    
    while (!ShouldCloseGame()) {
        UpdateGame();
        DrawGame();
    }
    
    CloseGame();
    return 0;
}