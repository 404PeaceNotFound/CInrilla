#ifndef CORE_PLATFORM_H
#define CORE_PLATFORM_H

#include <stdbool.h>

void Core_Init(void);
void Core_Close(void);
bool Core_ShouldClose(void);
double Core_GetTime(void); // Delta time
void Core_PlayMusic(void);
void Core_PlaySoundUI(void);

#endif