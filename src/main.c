#include "scenes/scene_manager.h"

int main(void) {
    // Inicializa Janela, Audio e o Gerenciador de Cenas
    InicializarGerenciadorCenas();
    
    while (!DeveFecharJogo()) {
        AtualizarGerenciadorCenas();
        DesenharGerenciadorCenas();
    }
    
    EncerrarGerenciadorCenas();
    return 0;
}