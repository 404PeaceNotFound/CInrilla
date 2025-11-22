#ifndef EVENT_BUS_H
#define EVENT_BUS_H

typedef enum {
    EVENT_START_GAME,
    EVENT_GAME_OVER,
    EVENT_COUNT
} EventType;

typedef struct {
    void* data;
} EventContext;

typedef void (*EventHandler)(EventContext context);

void EventBus_Init(void);
void EventBus_Subscribe(EventType type, EventHandler handler);
void EventBus_Publish(EventType type, EventContext context);

#endif