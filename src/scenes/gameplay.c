        #include "scene_manager.h"
        #include "../data/entity_types.h"
        #include "../data/map_data.h"
        #include "../systems/systems.h"
        #include "../entities/entities.h"
        #include "../config/config.h"
        #include "../systems/enemy_system.h"
        #include "../ui/ui.h"


        static Player player;
        static EnvItem envItems[] = {
            {{ 0, 0, 2280, 800 }, 0, DARKBLUE }, // "background"
            {{ 0, 400, 1280, 500 }, 1, DARKGREEN }, // chao 1
            {{ 1280, 450, 600, 400 }, 1, DARKGREEN }, // chao 2
            {{ 1880, 400, 400, 500 }, 1, DARKGREEN }, // chao 3
            {{ 250, 350, 100, 10 }, 1, DARKGREEN }, // retangulo alto 1
            {{ 650, 350, 100, 10 }, 1, DARKGREEN } // retangulo alto 2
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

        Rectangle atkRect = {
            attackX,
            p->position.y - 40,
            atkRange,
            atkHeight,
        };

    if (CheckCollisionRecs(atkRect, enemyRect)) {
            
            e->health -= p->damage; // Aplica o Dano real
            
            // Garante que o dano só seja aplicado uma vez por ataque
            p->hasHit = true; 
            
            // Aplica o Knockback Horizontal (Empurrão)
            if (e->health > 0) {
                // Determine a direção do empurrão
                float knockbackAmount = 40.0f; 
                e->position.x += (p->position.x < e->position.x) ? knockbackAmount : -knockbackAmount; 
                
                // Aplica Knockback Vertical (Pulo) se necessário
                e->verticalSpeed = -100.0f; // Exemplo de pulo de dano
            } else {
                 e->active = 0; // Temporário: Garante que o inimigo suma após a morte
            }
            
            return; // O inimigo foi atingido, não verifica mais colisões neste frame.
        }
    }

    // ---------------------------------------------------------
    // ETAPA 2: VERIFICA COLISÃO DE CORPO (Dano no Player)
    // ---------------------------------------------------------
    if (CheckCollisionRecs(playerBody, enemyRect)) {
            if (p->invulnerabilityTimer <= 0) 
            { 
        // PLAYER TOMOU DANO
                p->health -= e->damage; // Tira vida
                p->invulnerabilityTimer = 2.0f; // Tempo de invulnerabilidade
                p->canJump = false; // Força o estado aéreo para knockback

                // Empurrão no Player (Knockback)
                if (p->position.x < e->position.x) {
                    p->position.x -= 50; // Player vai para Esquerda
                } else {
                    p->position.x += 50; // Player vai para Direita
                }
                
                p->speed = -200; // Pulo pequeno de dano (Knockback vertical)
            }
        }   
    }

// init 
void Gameplay_Init(void) {
            Render_LoadAssets();
            //player

            player.speed = 0;
            player.canJump = false;
            player.isatk = false;

            initPlayer(&player);

            // camera 
            camera.target = player.position;
            camera.offset = (Vector2){LARGURA_TELA/2.0f, ALTURA_TELA/2.0f};
            camera.rotation = 0.0f;
            camera.zoom = 1.0f;

            // Geração de inimigos
            enemyCount = 3  ;
            ResetAllEnemies(enemies, enemyCount);
            
        }


EstadoJogo Gameplay_Update(void) { //Aqui rola a zuera de atualizar com o tempo
            static bool resetGame = true;

            // Reset an morte
            if (resetGame) {
                    //Reset Completo do Player 
                    player.health = 5;
                    player.state = PlayerIdle;
                    player.isatk = false;
                    player.speed = 0.0f;
                    player.canJump = true;
                    player.position = (Vector2){ 200, 400 };
                    player.anim[PlayerDead].final = false;  
                    ResetAllEnemies(enemies, enemyCount);  //reseta inimigos    
                    resetGame = false;
                }

            float dt = GetFrameTime();


            // 1. Entidade processa Input
            Entities_ProcessPlayerInput(&player, dt);
            
            // 2. Sistema de Física resolve colisões e movimento
            Physics_UpdatePlayer(&player, envItems, 5, dt);
            
            // 3. Atualiza Câmera
            int envItemsLength = sizeof(envItems)/sizeof(envItems[0]);
            Render_UpdateCamera(&camera, &player, envItems, envItemsLength, LARGURA_TELA, ALTURA_TELA);

            // 3. Atualiza Sprite
            Render_UpdateAnim(&player.anim[player.state], dt);
            
            if (IsKeyPressed(KEY_BACKSPACE)) return TELA_PAUSA;
            
            if (player.state == PlayerDead) {
                if (player.anim[player.state].final) {
                    resetGame = true;   
                    return TELA_GAMEOVER;
                    }

                    return TELA_GAMEPLAY;
                }
            
            if (IsKeyPressed(KEY_ESCAPE)){
                resetGame = true;
                return TELA_MENU;
            }

            if(player.anim[player.state].final){
                player.state = PlayerIdle;              
                }

            if (player.invulnerabilityTimer > 0) {
                player.invulnerabilityTimer -= dt;
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
                }
            EndMode2D();
            UI_DesenharHealthBar(player.health,player.health,LARGURA_TELA);
            DrawText("Controles: Setas + Espaco | ESC para voltar", 20, 20, 20, BLACK);
        }
