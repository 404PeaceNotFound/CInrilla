#include "event_bus.h"
#include <stdio.h>

#define MAX_LISTENERS 10

static EventHandler listeners[EVENT_COUNT][MAX_LISTENERS];
static int listener_counts[EVENT_COUNT];

void EventBus_Init(void) {
    for (int i = 0; i < EVENT_COUNT; i++) listener_counts[i] = 0;
}

void EventBus_Subscribe(EventType type, EventHandler handler) {
    if (type < 0 || type >= EVENT_COUNT) return;
    if (listener_counts[type] < MAX_LISTENERS) {
        listeners[type][listener_counts[type]++] = handler;
    }
}

void EventBus_Publish(EventType type, EventContext context) {
    if (type < 0 || type >= EVENT_COUNT) return;
    for (int i = 0; i < listener_counts[type]; i++) {
        if (listeners[type][i]) listeners[type][i](context);
    }
}