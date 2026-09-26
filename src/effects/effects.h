#ifndef CSAFEMEMZ_EFFECTS_H
#define CSAFEMEMZ_EFFECTS_H

#include <windows.h>

void EffectsInit(HWND hwnd);

void EffectGlitch(
    HDC hdc,
    int width,
    int height,
    DWORD elapsed
);

#endif