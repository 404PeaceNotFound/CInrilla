#include "scene_manager.h"
#include "../systems/systems.h"
#include "../ui/ui.h"
#include "../config/config.h"
#include "../core/core_platform.h"
#include <stddef.h>

static AnimacaoSpritesheet animMenu;
static bool menuinicializado = false;
static BotaoUI btnJogar = {{LARGURA_TELA/2 - 100, 300, 200, 50}, "JOGAR", false};
static BotaoUI btnCreditos = {{LARGURA_TELA/2 - 100, 400, 200, 50}, "CRÉDITOS", false};
static Music musicaMenu;

void Menu_Init(void) {
    if (menuinicializado) return;

    // Carrega a animação
    animMenu = Render_CreateAnim("assets/sprites/menu/menu_spritesheet.png", 11, 11, 30, LARGURA_TELA, ALTURA_TELA, true, true);
    
    // Carrega a música (Caminho que vimos no log funcionar)
    if (FileExists("assets/sounds/OST/02 - Title Theme.ogg")) {
        musicaMenu = LoadMusicStream("assets/sounds/OST/02 - Title Theme.ogg");
        
        if (musicaMenu.ctxData != NULL) {
            musicaMenu.looping = true;
            PlayMusicStream(musicaMenu);
            SetMasterVolume(1.0f);
        }
    }
    
    menuinicializado = true;
}
EstadoJogo Menu_Update(void) {
    Vector2 mouse = GetMousePosition();
    
    // 1. ÁUDIO: Perfeito, mantém a música rodando
    if (musicaMenu.ctxData != NULL) {
        UpdateMusicStream(musicaMenu); 
    }
    
    Render_UpdateAnim(&animMenu, GetFrameTime());
    
    btnJogar.estaSelecionado = CheckCollisionPointRec(mouse, btnJogar.retangulo);
    btnCreditos.estaSelecionado = CheckCollisionPointRec(mouse, btnCreditos.retangulo);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (btnJogar.estaSelecionado) {
            // --- IMPORTANTE: PARE A MÚSICA AQUI ---
            Gameplay_Reiniciar();
            if (musicaMenu.ctxData != NULL) StopMusicStream(musicaMenu);
            // --------------------------------------
            return TELA_GAMEPLAY;
        }
        
        if (btnCreditos.estaSelecionado) {
            // Opcional: Se quiser parar nos créditos também
            // if (musicaMenu.ctxData != NULL) StopMusicStream(musicaMenu);
            return TELA_CREDITOS;
        }
    }
    return TELA_MENU;
}

void Menu_Draw(void) {
    ClearBackground(RAYWHITE);
    Render_DrawAnim(animMenu, (Vector2){LARGURA_TELA/2.0f, ALTURA_TELA/2.0f}, false);


    UI_DesenharTextoCentralizado(TITULO_JOGO, 100, 60, WHITE);
    UI_DesenharBotao(&btnJogar);
    UI_DesenharBotao(&btnCreditos);
}

void Menu_Deinit(void) {
    if(menuinicializado) {
        Render_UnloadAnim(&animMenu);
        menuinicializado = false;
    }
}