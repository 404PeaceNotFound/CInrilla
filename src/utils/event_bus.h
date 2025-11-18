#ifndef EVENT_BUS_H
#define EVENT_BUS_H

// Lista de todos os eventos possíveis no jogo
typedef enum {
    EVENT_PLAYER_HIT,
    EVENT_ENEMY_DIED,
    EVENT_ITEM_PICKUP,
    EVENT_GAME_OVER,
    // Adicione novos eventos aqui
    EVENT_COUNT
} EventType;

// Estrutura de dados genérica enviada junto com o evento
typedef struct {
    void* data; // Pode ser um ponteiro para o Player, int de dano, etc.
} EventContext;

// Ponteiro de função: quem quiser ouvir eventos deve ter uma função assim
typedef void (*EventHandler)(EventContext context);

// Inicializa o sistema
void EventBus_Init();

// Inscreve uma função para ouvir um tipo específico de evento
void EventBus_Subscribe(EventType type, EventHandler handler);

// Dispara um evento para todos os ouvintes inscritos
void EventBus_Publish(EventType type, EventContext context);

#endif