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

void EffectShake(
    HDC hdc,
    int width,
    int height,
    DWORD elapsed
);

void EffectTear(
    HDC hdc,
    int width,
    int height,
    DWORD elapsed
);

void EffectColorShift(
    HDC hdc,
    int width,
    int height,
    DWORD elapsed
);

void EffectRects(
    HDC hdc,
    int width,
    int height,
    DWORD elapsed
);

void EffectFlash(
    HDC hdc,
    int width,
    int height,
    DWORD elapsed
);

void EffectChaos(
    HDC hdc,
    int width,
    int height,
    DWORD elapsed,
    int intensity
);

void EffectSwirl(
    HDC hdc,
    int width,
    int height,
    DWORD elapsed
);

void EffectScanlines(
    HDC hdc,
    int width,
    int height,
    DWORD elapsed
);

void EffectPixelate(
    HDC hdc,
    int width,
    int height,
    DWORD elapsed
);

void EffectMirror(
    HDC hdc,
    int width,
    int height,
    DWORD elapsed
);

void EffectRGBSplit(
    HDC hdc,
    int width,
    int height,
    DWORD elapsed
);

#endif