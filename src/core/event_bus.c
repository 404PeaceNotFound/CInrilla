#include "event_bus.h"
#include <stdio.h>

#define MAX_LISTENERS_PER_EVENT 10

// Array de listas de ouvintes. 
// events[EVENT_PLAYER_HIT] contém a lista de funções que querem saber disso.
static EventHandler listeners[EVENT_COUNT][MAX_LISTENERS_PER_EVENT];
static int listener_counts[EVENT_COUNT];

void EventBus_Init() {
    for (int i = 0; i < EVENT_COUNT; i++) {
        listener_counts[i] = 0;
    }
}

void EventBus_Subscribe(EventType type, EventHandler handler) {
    if (type < 0 || type >= EVENT_COUNT) return;
    
    int count = listener_counts[type];
    if (count < MAX_LISTENERS_PER_EVENT) {
        listeners[type][count] = handler;
        listener_counts[type]++;
    } else {
        printf("ERRO: Maximo de ouvintes atingido para o evento %d\n", type);
    }
}

void EventBus_Publish(EventType type, EventContext context) {
    if (type < 0 || type >= EVENT_COUNT) return;

    // Percorre todas as funções inscritas neste evento e as executa
    for (int i = 0; i < listener_counts[type]; i++) {
        EventHandler handler = listeners[type][i];
        if (handler) {
            handler(context);
        }
    }
}