#ifndef CSAFEMEMZ_EFFECTS_H
#define CSAFEMEMZ_EFFECTS_H

#include <windows.h>

void EffectsInit(HWND hwnd);
void EffectsShutdown(void);

void EffectGlitch(HDC hdc, int width, int height, DWORD elapsed);
void EffectShake(HDC hdc, int width, int height, DWORD elapsed);
void EffectTear(HDC hdc, int width, int height, DWORD elapsed);
void EffectColorShift(HDC hdc, int width, int height, DWORD elapsed);
void EffectRects(HDC hdc, int width, int height, DWORD elapsed);
void EffectFlash(HDC hdc, int width, int height, DWORD elapsed);
void EffectSwirl(HDC hdc, int width, int height, DWORD elapsed);
void EffectScanlines(HDC hdc, int width, int height, DWORD elapsed);
void EffectPixelate(HDC hdc, int width, int height, DWORD elapsed);
void EffectMirror(HDC hdc, int width, int height, DWORD elapsed);
void EffectRGBSplit(HDC hdc, int width, int height, DWORD elapsed);
void EffectWave(HDC hdc, int width, int height, DWORD elapsed);
void EffectInvert(HDC hdc, int width, int height, DWORD elapsed);
void EffectVerticalTear(HDC hdc, int width, int height, DWORD elapsed);
void EffectBars(HDC hdc, int width, int height, DWORD elapsed);
void EffectChecker(HDC hdc, int width, int height, DWORD elapsed);
void EffectZoom(HDC hdc, int width, int height, DWORD elapsed);
void EffectNoise(HDC hdc, int width, int height, DWORD elapsed);
void EffectWarp(HDC hdc, int width, int height, DWORD elapsed);
void EffectColorBands(HDC hdc, int width, int height, DWORD elapsed);
void EffectSpiral(HDC hdc, int width, int height, DWORD elapsed);
void EffectFlicker(HDC hdc, int width, int height, DWORD elapsed);
void EffectVignette(HDC hdc, int width, int height, DWORD elapsed);
void EffectMosaic(HDC hdc, int width, int height, DWORD elapsed);
void EffectCenterSplit(HDC hdc, int width, int height, DWORD elapsed);
void EffectRandomLines(HDC hdc, int width, int height, DWORD elapsed);
void EffectFinalChaos(HDC hdc, int width, int height, DWORD elapsed, int intensity);

#endif
