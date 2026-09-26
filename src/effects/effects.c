#include "effects.h"

#include <stdlib.h>
#include <windows.h>

static HWND g_window = NULL;

void EffectsInit(HWND hwnd)
{
    g_window = hwnd;
}

/* =========================================================
   GLITCH
   ========================================================= */

void EffectGlitch(
    HDC hdc,
    int width,
    int height,
    DWORD elapsed
)
{
    int offset = (int)((elapsed / 40) % 20);

    if ((elapsed / 100) % 2 == 0)
    {
        BitBlt(
            hdc,
            offset,
            0,
            width - offset,
            height,
            hdc,
            0,
            0,
            SRCCOPY
        );
    }
    else
    {
        BitBlt(
            hdc,
            0,
            0,
            width - offset,
            height,
            hdc,
            offset,
            0,
            SRCCOPY
        );
    }
}

/* =========================================================
   1. SHAKE
   ========================================================= */

void EffectShake(
    HDC hdc,
    int width,
    int height,
    DWORD elapsed
)
{
    int offsetX =
        (int)((elapsed / 17) % 31) - 15;

    int offsetY =
        (int)((elapsed / 23) % 25) - 12;

    BitBlt(
        hdc,
        offsetX,
        offsetY,
        width - abs(offsetX),
        height - abs(offsetY),
        hdc,
        0,
        0,
        SRCCOPY
    );
}

/* =========================================================
   2. HORIZONTAL TEAR
   ========================================================= */

void EffectTear(
    HDC hdc,
    int width,
    int height,
    DWORD elapsed
)
{
    int seed =
        (int)(elapsed / 30);

    srand((unsigned int)seed);

    int bands = 12;

    for (int i = 0; i < bands; ++i)
    {
        int y =
            rand() % height;

        int bandHeight =
            2 + rand() % 35;

        int offset =
            (rand() % 81) - 40;

        if (y + bandHeight > height)
            bandHeight = height - y;

        if (bandHeight <= 0)
            continue;

        BitBlt(
            hdc,
            offset,
            y,
            width - abs(offset),
            bandHeight,
            hdc,
            0,
            y,
            SRCCOPY
        );
    }
}

/* =========================================================
   3. COLOR SHIFT
   ========================================================= */

void EffectColorShift(
    HDC hdc,
    int width,
    int height,
    DWORD elapsed
)
{
    int offset =
        (int)((elapsed / 20) % 25) - 12;

    /*
     * Сдвигаем цветовые ощущения через
     * полупрозрачные цветные полосы.
     *
     * Само изображение рабочего стола
     * не сохраняется и не записывается на диск.
     */

    int bandHeight = 3;

    HBRUSH redBrush =
        CreateSolidBrush(RGB(255, 0, 0));

    HBRUSH cyanBrush =
        CreateSolidBrush(RGB(0, 255, 255));

    for (int y = 0; y < height; y += bandHeight * 8)
    {
        RECT redRect =
        {
            offset,
            y,
            width,
            y + bandHeight
        };

        RECT cyanRect =
        {
            -offset,
            y + bandHeight * 3,
            width,
            y + bandHeight * 4
        };

        FrameRect(
            hdc,
            &redRect,
            redBrush
        );

        FrameRect(
            hdc,
            &cyanRect,
            cyanBrush
        );
    }

    DeleteObject(redBrush);
    DeleteObject(cyanBrush);
}

/* =========================================================
   4. RANDOM RECTANGLES
   ========================================================= */

void EffectRects(
    HDC hdc,
    int width,
    int height,
    DWORD elapsed
)
{
    srand(
        (unsigned int)(elapsed / 40)
    );

    for (int i = 0; i < 35; ++i)
    {
        int x =
            rand() % width;

        int y =
            rand() % height;

        int rectWidth =
            10 + rand() % 180;

        int rectHeight =
            2 + rand() % 45;

        if (x + rectWidth > width)
            rectWidth = width - x;

        if (y + rectHeight > height)
            rectHeight = height - y;

        if (rectWidth <= 0 ||
            rectHeight <= 0)
        {
            continue;
        }

        int r = rand() % 256;
        int g = rand() % 256;
        int b = rand() % 256;

        HBRUSH brush =
            CreateSolidBrush(
                RGB(r, g, b)
            );

        RECT rect =
        {
            x,
            y,
            x + rectWidth,
            y + rectHeight
        };

        FillRect(
            hdc,
            &rect,
            brush
        );

        DeleteObject(brush);
    }
}

/* =========================================================
   5. FLASH
   ========================================================= */

void EffectFlash(
    HDC hdc,
    int width,
    int height,
    DWORD elapsed
)
{
    /*
     * Короткие белые вспышки.
     */

    if ((elapsed % 700) < 90)
    {
        HBRUSH white =
            CreateSolidBrush(
                RGB(255, 255, 255)
            );

        RECT screen =
        {
            0,
            0,
            width,
            height
        };

        FillRect(
            hdc,
            &screen,
            white
        );

        DeleteObject(white);
    }
}