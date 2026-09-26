#ifndef CSAFEMEMZ_EFFECTS_H
#define CSAFEMEMZ_EFFECTS_H

#include <windows.h>

/* Инициализация системы эффектов */
void EffectsInit(HWND hwnd);

/* Базовый glitch */
void EffectGlitch(
    HDC hdc,
    int width,
    int height,
    DWORD elapsed
);

/* 1. Тряска экрана */
void EffectShake(
    HDC hdc,
    int width,
    int height,
    DWORD elapsed
);

/* 2. Горизонтальные разрывы */
void EffectTear(
    HDC hdc,
    int width,
    int height,
    DWORD elapsed
);

/* 3. Цветовой сдвиг */
void EffectColorShift(
    HDC hdc,
    int width,
    int height,
    DWORD elapsed
);

/* 4. Хаотичные прямоугольные фрагменты */
void EffectRects(
    HDC hdc,
    int width,
    int height,
    DWORD elapsed
);

/* 5. Вспышка */
void EffectFlash(
    HDC hdc,
    int width,
    int height,
    DWORD elapsed
);

#endif