#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "raylib.h"

// APENAS estas funções:
void InitGraphics(void);
void DrawMenuBackground(void);
void DrawFooter(const char* text);
void CloseGraphics(void);

#endif