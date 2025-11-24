#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

typedef enum {
    TELA_MENU,
    TELA_GAMEPLAY,
    TELA_CREDITOS,
    TELA_SAIR,
    TELA_PAUSA,
    TELA_GAMEOVER,
    TELA_WINNER
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

//funcoes pausa
void Pausa_Init(void);
EstadoJogo Pausa_Update(void);
void Pausa_Draw(void);
void Pausa_Deinit(void);

// funcoes gameplay
void Gameplay_Init(void);
EstadoJogo Gameplay_Update(void);
void Gameplay_Draw(void);

// funcoes credits
void Creditos_Init(void);
EstadoJogo Creditos_Update(void);
void Creditos_Draw(void);
void Creditos_Deinit(void);

// funcoes gameover
void GameOver_Init(void);
EstadoJogo GameOver_Update(void);
void GameOver_Draw(void);
void GameOver_Deinit(void); 

// funcoes winner
void Winner_Init(void);
EstadoJogo Winner_Update(void);
void Winner_Draw(void);
void Winner_Deinit(void);

#endif