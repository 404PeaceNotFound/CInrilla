#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

typedef enum {
    TELA_MENU,
    TELA_GAMEPLAY,
    TELA_CREDITOS,
    TELA_SAIR
} EstadoJogo;

// funcoes scene_manager
void SM_Init(void);
void SM_Update(void);
void SM_Draw(void);
void SM_Deinit(void);

// funcoes menu
void Menu_Init(void);
EstadoJogo Menu_Update(void);
void Menu_Draw(void);
void Menu_Deinit(void);

// funcoes gameplay
void Gameplay_Init(void);
EstadoJogo Gameplay_Update(void);
void Gameplay_Draw(void);

// funcoes credits
void Creditos_Init(void);
EstadoJogo Creditos_Update(void);
void Creditos_Draw(void);
void Creditos_Deinit(void);

#endif