        #include "scene_manager.h"
        #include "../data/entity_types.h"
        #include "../data/map_data.h"
        #include "../systems/systems.h"
        #include "../entities/entities.h"
        #include "../config/config.h"
        #include "../systems/enemy_system.h"
        #include "../ui/ui.h"

        // player 
        static Player player;
        static EnvItem envItems[] = {
            {{ 0, 0, 1000, 400 }, 0, LIGHTGRAY },
            {{ 0, 400, 1000, 200 }, 1, GRAY },
            {{ 300, 250, 400, 10 }, 1, GRAY },
            {{ 250, 320, 100, 10 }, 1, GRAY },
            {{ 650, 320, 100, 10 }, 1, GRAY }
        };
        static Camera2D camera;

        // inimigos

        #define MAX_ENEMIES 10 
        static Enemy enemies[MAX_ENEMIES];
        static int enemyCount =0;
        //colição inimigos x player

void CheckPlayerEnemyCollision(Player *p, Enemy *e) {
    if (!e->active) return;

    // 1. Áreas de Colisão
    Rectangle playerBody = { p->position.x - 20, p->position.y - 40, 40, 40 }; 
    Rectangle enemyRect = { e->position.x, e->position.y - e->height, e->width, e->height };

    // ---------------------------------------------------------
    // ETAPA 1: VERIFICA O ATAQUE (Hitbox da Arma)
    // ---------------------------------------------------------
    // ATENÇÃO: Isso agora fica FORA do CheckCollisionRecs(playerBody...)
    if(p->isatk && !p->hasHit){
        float atkRange = 50.0f;
        float atkHeight = 40.0f;
        float attackX;

        // Define onde a espada "nasce"
        if(p->PlayerDirection == 1){
            attackX = p->position.x + 20; // Na frente (Direita)
        }
        else {
            attackX = p->position.x - 20 - atkRange; // Na frente (Esquerda)
        }

    if (IsKeyPressed(KEY_BACKSPACE)) return TELA_PAUSA;
    if (player.health <= 0) return TELA_GAMEOVER;
        Rectangle atkRect = {
            attackX,
            p->position.y - 40,
            atkRange,
            atkHeight,
        };

if (CheckCollisionRecs(atkRect, enemyRect)) {
        
        // 1. APLICA KNOCKBACK HORIZONTAL (Posição)
        if (p->position.x < e->position.x) {
            e->position.x += 30; // Empurra para Direita
        } else {
            e->position.x -= 30; // Empurra para Esquerda
        }
        
        // 2. APLICA KNOCKBACK VERTICAL (Pulo de dor)
        // ERRADO: e->speed = -150; (Isso quebra a caminhada)
        
        // CERTO: Use verticalSpeed (se sua struct Enemy tiver essa variável)
        // Se não tiver, verifique como você fez a gravidade do inimigo no Physics.
        // Baseado no seu código anterior, você usou 'verticalSpeed' na física.
        e->verticalSpeed = -100.0f; 
        
        // 3. TRAVA O ATAQUE
        p->hasHit = true; 
        
        return; 
    }
    }

    // ---------------------------------------------------------
    // ETAPA 2: VERIFICA COLISÃO DE CORPO (Dano no Player)
    // ---------------------------------------------------------
    if (CheckCollisionRecs(playerBody, enemyRect)) {
        
        bool isFalling = p->speed > 0;
        bool isAbove = (p->position.y - 10) < enemyRect.y + (enemyRect.height * 0.5f);

        //if (isFalling && isAbove) {
            // MATOU O INIMIGO (Pulo Mario)
            //e->active = 0;      
            //p->speed = -400.0f; 
        //} 
        //else {
            // PLAYER TOMOU DANO
            p->health -= 1; // Tira vida

            // Empurrão no Player (para o player não ficar grudado no inimigo)
            if (p->position.x < e->position.x) {
                p->position.x -= 50; // Player vai para Esquerda
            } else {
                p->position.x += 50; // Player vai para Direita
            }
            
            p->speed = -200; // Pulo pequeno de dano
       // }
    }
}
        // init 
        void Gameplay_Init(void) {
            Render_LoadAssets();
            //player
            player.position = (Vector2){ 600, 300 };
            player.speed = 0;
            player.canJump = false;
            player.isatk = false;

            initPlayer(&player);

            // camera 
            camera.target = player.position;
            camera.offset = (Vector2){LARGURA_TELA/2.0f, ALTURA_TELA/2.0f};
            camera.rotation = 0.0f;
            camera.zoom = 1.5f;

            // Geração de inimigos
            enemyCount = 0;

            // --- INIMIGO 1: Javali (Boar) ---
            enemies[enemyCount] = Enemy_Create((Vector2){500, 400}, 200, 500, 60); 
            // AQUI ESTÁ A MÁGICA QUE FALTAVA:
            Render_ConfigEnemy(&enemies[enemyCount], ENEMY_TYPE_BOAR); 
            enemyCount++;

            // --- INIMIGO 2: Javali (Boar) ---
            enemies[enemyCount] = Enemy_Create((Vector2){800, 400}, 700, 900, 60);
            Render_ConfigEnemy(&enemies[enemyCount], ENEMY_TYPE_BOAR);
            enemyCount++;

            // --- INIMIGO 3: Abelha (Bee) ---
            enemies[enemyCount] = Enemy_Create((Vector2){800, 200}, 200, 500, 60); 
            // Configura como Abelha
            Render_ConfigEnemy(&enemies[enemyCount], ENEMY_TYPE_SMALL_BEE);
            enemyCount++;

            
        }


        EstadoJogo Gameplay_Update(void) { //Aqui rola a zuera de atualizar com o tempo


            float dt = GetFrameTime();


            // 1. Entidade processa Input
            Entities_ProcessPlayerInput(&player, dt);
            
            // 2. Sistema de Física resolve colisões e movimento
            Physics_UpdatePlayer(&player, envItems, 5, dt);
            
            // 3. Atualiza Câmera
            Render_UpdateCamera(&camera, &player, LARGURA_TELA, ALTURA_TELA);

            // 3. Atualiza Sprite
            Render_UpdateAnim(&player.anim[player.state], dt);

            if (IsKeyPressed(KEY_ESCAPE)) return TELA_MENU;

            if(player.anim[player.state].final){
                player.state = PlayerIdle;
                }

            // 5. Atualização dos Inimigos
            for(int i = 0; i < enemyCount; i++){
                // Só processa se o inimigo estiver ativo
                if (enemies[i].active) {
                    Physics_UpdateEnemy(&enemies[i], envItems, 5, dt);
                    UpdateEnemy(&enemies[i], dt);
                    CheckPlayerEnemyCollision(&player, &enemies[i]);
                }
            }

            return TELA_GAMEPLAY;
            }


        //draw 
        void Gameplay_Draw() {
            ClearBackground(LIGHTGRAY);
            
            BeginMode2D(camera);
                Render_Map(envItems, 5);
                Render_Player(&player);
                //render inimigos
                for(int i =0; i < enemyCount; i++){
                    Render_Enemy(&enemies[i]);
                    //DrawRectangle(enemies[i].position.x, enemies[i].position.y, 40, 40, RED); testes visuais
                    //DrawCircle(enemies[i].position.x, enemies[i].position.y, 10, GREEN);
                }
            EndMode2D();
            UI_DesenharHealthBar(player.health,50,LARGURA_TELA);
            //DrawText(TextFormat("Vida: %d", player.health), 20, 50, 20, RED);
            DrawText("Controles: Setas + Espaco | ESC para voltar", 20, 20, 20, BLACK);
        }
