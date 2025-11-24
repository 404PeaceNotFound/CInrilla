        #include "scene_manager.h"
        #include "../data/entity_types.h"
        #include "../data/map_data.h"
        #include "../systems/systems.h"
        #include "../entities/entities.h"
        #include "../config/config.h"
        #include "../systems/enemy_system.h"

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

            // Hitbox do Player (ajustada para ser menor que o sprite)
            Rectangle playerRect = { p->position.x - 20, p->position.y - 40, 40, 40 }; 
            // Hitbox do Inimigo
            Rectangle enemyRect = { e->position.x, e->position.y - e->height, e->width, e->height };

            if (CheckCollisionRecs(playerRect, enemyRect)) {
                
                // LÓGICA: Pulo na Cabeça (Mario Style)
                // Se o player está caindo (speed > 0) E os pés estão acima do centro do inimigo
                bool isFalling = p->speed > 0;
                bool isAbove = (p->position.y - 10) < enemyRect.y + (enemyRect.height * 0.5f);

                if(p->isatk){
                    float atkRange = 50.0f;
                    float atkHeight = 40.0f;

                    float attackX;
                    if(p->PlayerDirection == 1){
                        attackX = p->position.x + 20;
                    }
                    else if(p->PlayerDirection == -1){
                        attackX = p->position.x - 20 - atkRange;
                    }

                    Rectangle atkRect = {
                        attackX,
                        p->position.y - 40,
                        atkRange,
                        atkHeight,
                    };

                    


                    if (p->position.x < e->position.x) {
                        e->position.x -= 50; // Joga pra esquerda
                        //e->position.x = 0; inimigo voltaria
                    } else {
                        e->position.x += 50; // Joga pra direita
                        //e->position.x = 0;
                    }
                    
                    e->speed = -100; // Pulo pequeno de dano
       
                }

                else if (isFalling && isAbove) {
                    // MATOU O INIMIGO
                    e->active = 0;      // Desativa o inimigo
                    p->speed = -400.0f; // Pulo rebote (pula de novo automaticamente)
                } 
                else {
                    // PLAYER TOMOU DANO (Empurrão / Knockback)
                    p->health -= 1;
                    if (p->position.x < e->position.x) {
                        p->position.x -= 50; // Joga pra esquerda
                        //e->position.x = 0; inimigo voltaria
                    } else {
                        p->position.x += 50; // Joga pra direita
                        //e->position.x = 0;
                    }
                    
                    p->speed = -100; // Pulo pequeno de dano
                    
                }
            }
        }
        // init 
        void Gameplay_Init(void) {
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

            //geração de inimigos

            enemyCount = 0;

            enemies[enemyCount++] = Enemy_Create((Vector2){500, 400}, 200, 500, 60); // inimigos no chão y=400
            enemies[enemyCount++] = Enemy_Create((Vector2){800, 200}, 200, 500, 60); // inmigo teste de colição entre inimigos

            float plataformay = envItems[2].rect.y;
            enemies[enemyCount++]= Enemy_Create((Vector2){450, plataformay}, 300, 650, 80); // inimigos na plataforma 

            
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
                    DrawEnemy(&enemies[i]);
                    //DrawRectangle(enemies[i].position.x, enemies[i].position.y, 40, 40, RED); testes visuais
                    //DrawCircle(enemies[i].position.x, enemies[i].position.y, 10, GREEN);
                }
            EndMode2D();
            
            DrawText(TextFormat("Vida: %d", player.health), 20, 50, 20, RED);
            DrawText("Controles: Setas + Espaco | ESC para voltar", 20, 20, 20, BLACK);
        }