#include <raylib.h>
#include <stdio.h>
#include "video_sheet.h"


void TelaCreditos(void) {
    const int screenWidth = 1200;
    const int screenHeight = 720;

    VideoSheet bg = LoadVideoSheet(
        "assets/frames/sheet.png",  // arquivo sheet
        screenWidth,     // Largura de cada frame
        screenHeight,      // Altura de cada frame  
        66,      // Total de frames
        24.0f     // FPS da animação
    );

    float scrollY = screenHeight;
    bool sair = false;

    Rectangle btnback  = {1000, 650, 175, 50}; // v_locais 
    Color CorBotao = { 30, 30, 30, 150 };

    Sound Som_UI = LoadSound("assets/sounds/UI_SOUND.mp3");
    SetSoundVolume(Som_UI, 0.1f);
    

    //LOOP
    while (!WindowShouldClose() && !sair) { 
        float dt = GetFrameTime();

        UpdateVideoSheetSmooth(&bg, dt);

        // Lógica específica dos créditos (scroll de texto)
        scrollY -= 60.0f * dt;
        if(scrollY < -400) scrollY = screenHeight; //desce os creditos

        if(IsKeyPressed(KEY_ESCAPE)) sair = true;

        BeginDrawing();
        ClearBackground(BLACK);

        DrawVideoSheetSmooth(&bg, (Vector2){0,0}); // desenha o “vídeo”
        DrawRectangleRec(btnback, CorBotao);

        //textos na pagina de creditos
        DrawText("CRÉDITOS", 290, scrollY-6, 50, BLACK);
        DrawText("CRÉDITOS", 290, scrollY-10, 50, WHITE);

        //Nome dos colaboradores
        DrawText("Equipe:", 240, scrollY+42, 30, BLACK);
        DrawText("Equipe:", 238, scrollY+38, 30, WHITE);

        DrawText("Gabriel Vieira <gvnrb>", 240, scrollY+82, 30, BLACK);
        DrawText("Gabriel Vieira <gvnrb>", 238, scrollY+78, 30, WHITE);

        DrawText("Guilherme Vitor <gvca>",240, scrollY+122, 30, BLACK);
        DrawText("Guilherme Vitor <gvca>",238, scrollY+118, 30, WHITE);

        DrawText("João victor <jvajs>", 240, scrollY+162, 30, BLACK);
        DrawText("João victor <jvajs>", 238, scrollY+158, 30, WHITE);

        DrawText("José Lucas <jlhm>", 240, scrollY+202, 30, BLACK);
        DrawText("José Lucas <jlhm>", 238, scrollY+198, 30, WHITE);

        DrawText("Luciano José <ljsj>", 240, scrollY+242, 30, BLACK);
        DrawText("Luciano José <ljsj>", 238, scrollY+238, 30, WHITE);

        // Agradecimentos 

        DrawText("Obrigado Por Jogar!", 240, scrollY+362, 30, BLACK);
        DrawText("Obrigado Por Jogar!", 238, scrollY+358, 30, WHITE);

        //verifica click no botao Voltar
        if(CheckCollisionPointRec(GetMousePosition(), btnback)){
            DrawRectangleRec(btnback, Fade(LIGHTGRAY, 0.8f));
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                sair = true;                                               
            }
        }
        DrawText("VOLTAR", btnback.x+45, btnback.y+15, 20, WHITE);
        EndDrawing();
    }

    //StopMusicStream(music);
    UnloadSound(Som_UI);
    UnloadVideoSheet(&bg);

}
