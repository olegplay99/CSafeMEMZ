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
    int offset = (int)((elapsed / 40) % 30) - 15;

    if (offset >= 0)
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
            width + offset,
            height,
            hdc,
            -offset,
            0,
            SRCCOPY
        );
    }
}

/* =========================================================
   SHAKE
   ========================================================= */

void EffectShake(
    HDC hdc,
    int width,
    int height,
    DWORD elapsed
)
{
    int offsetX =
        (int)((elapsed / 17) % 25) - 12;

    int offsetY =
        (int)((elapsed / 23) % 21) - 10;

    int copyWidth = width - abs(offsetX);
    int copyHeight = height - abs(offsetY);

    if (copyWidth <= 0 || copyHeight <= 0)
        return;

    BitBlt(
        hdc,
        offsetX,
        offsetY,
        copyWidth,
        copyHeight,
        hdc,
        offsetX < 0 ? -offsetX : 0,
        offsetY < 0 ? -offsetY : 0,
        SRCCOPY
    );
}

/* =========================================================
   TEAR
   ========================================================= */

void EffectTear(
    HDC hdc,
    int width,
    int height,
    DWORD elapsed
)
{
    srand((unsigned int)(elapsed / 35));

    for (int i = 0; i < 16; ++i)
    {
        int y = rand() % height;
        int bandHeight = 3 + rand() % 30;
        int offset = (rand() % 101) - 50;

        if (y + bandHeight > height)
            bandHeight = height - y;

        if (bandHeight <= 0)
            continue;

        if (offset >= 0)
        {
            BitBlt(
                hdc,
                offset,
                y,
                width - offset,
                bandHeight,
                hdc,
                0,
                y,
                SRCCOPY
            );
        }
        else
        {
            BitBlt(
                hdc,
                0,
                y,
                width + offset,
                bandHeight,
                hdc,
                -offset,
                y,
                SRCCOPY
            );
        }
    }
}

/* =========================================================
   COLOR SHIFT
   ========================================================= */

void EffectColorShift(
    HDC hdc,
    int width,
    int height,
    DWORD elapsed
)
{
    int offset =
        (int)((elapsed / 25) % 25) - 12;

    HPEN red =
        CreatePen(PS_SOLID, 2, RGB(255, 0, 0));

    HPEN cyan =
        CreatePen(PS_SOLID, 2, RGB(0, 255, 255));

    if (!red || !cyan)
    {
        if (red)
            DeleteObject(red);

        if (cyan)
            DeleteObject(cyan);

        return;
    }

    HPEN oldPen =
        (HPEN)SelectObject(hdc, red);

    for (int y = 0; y < height; y += 32)
    {
        MoveToEx(hdc, offset, y, NULL);
        LineTo(hdc, width, y);
    }

    SelectObject(hdc, cyan);

    for (int y = 16; y < height; y += 32)
    {
        MoveToEx(hdc, -offset, y, NULL);
        LineTo(hdc, width, y);
    }

    SelectObject(hdc, oldPen);

    DeleteObject(red);
    DeleteObject(cyan);
}

/* =========================================================
   RANDOM RECTANGLES
   ========================================================= */

void EffectRects(
    HDC hdc,
    int width,
    int height,
    DWORD elapsed
)
{
    srand((unsigned int)(elapsed / 45));

    for (int i = 0; i < 30; ++i)
    {
        int x = rand() % width;
        int y = rand() % height;

        int w = 10 + rand() % 180;
        int h = 3 + rand() % 40;

        if (x + w > width)
            w = width - x;

        if (y + h > height)
            h = height - y;

        if (w <= 0 || h <= 0)
            continue;

        HBRUSH brush =
            CreateSolidBrush(
                RGB(
                    rand() % 256,
                    rand() % 256,
                    rand() % 256
                )
            );

        if (!brush)
            continue;

        RECT rect =
        {
            x,
            y,
            x + w,
            y + h
        };

        FillRect(hdc, &rect, brush);

        DeleteObject(brush);
    }
}

/* =========================================================
   FLASH
   ========================================================= */

void EffectFlash(
    HDC hdc,
    int width,
    int height,
    DWORD elapsed
)
{
    /*
     * Короткая, не постоянная вспышка.
     * Делаем её серой, а не ярко-белой.
     */

    if ((elapsed % 900) >= 70)
        return;

    HBRUSH brush =
        CreateSolidBrush(RGB(180, 180, 180));

    if (!brush)
        return;

    RECT rect =
    {
        0,
        0,
        width,
        height
    };

    FillRect(hdc, &rect, brush);

    DeleteObject(brush);
}

/* =========================================================
   CHAOS
   ========================================================= */

void EffectChaos(
    HDC hdc,
    int width,
    int height,
    DWORD elapsed,
    int intensity
)
{
    if (intensity < 0)
        intensity = 0;

    if (intensity > 100)
        intensity = 100;

    if (intensity >= 10)
        EffectGlitch(hdc, width, height, elapsed);

    if (intensity >= 25)
        EffectShake(hdc, width, height, elapsed);

    if (intensity >= 40)
        EffectTear(hdc, width, height, elapsed);

    if (intensity >= 55)
        EffectColorShift(hdc, width, height, elapsed);

    if (intensity >= 70)
        EffectRects(hdc, width, height, elapsed);

    /*
     * Не добавляем сильную вспышку в хаос.
     */
}

/* =========================================================
   SWIRL
   ========================================================= */

void EffectSwirl(
    HDC hdc,
    int width,
    int height,
    DWORD elapsed
)
{
    int bandHeight = 24;

    int shift =
        (int)((elapsed / 18) % 120) - 60;

    for (int y = 0; y < height; y += bandHeight)
    {
        int h = bandHeight;

        if (y + h > height)
            h = height - y;

        if (h <= 0)
            continue;

        int offset =
            ((y / bandHeight) % 2)
                ? shift
                : -shift;

        if (offset >= 0)
        {
            BitBlt(
                hdc,
                offset,
                y,
                width - offset,
                h,
                hdc,
                0,
                y,
                SRCCOPY
            );
        }
        else
        {
            BitBlt(
                hdc,
                0,
                y,
                width + offset,
                h,
                hdc,
                -offset,
                y,
                SRCCOPY
            );
        }
    }
}

/* =========================================================
   SCANLINES
   ========================================================= */

void EffectScanlines(
    HDC hdc,
    int width,
    int height,
    DWORD elapsed
)
{
    (void)elapsed;

    HBRUSH brush =
        CreateSolidBrush(RGB(0, 0, 0));

    if (!brush)
        return;

    for (int y = 0; y < height; y += 6)
    {
        RECT line =
        {
            0,
            y,
            width,
            y + 2
        };

        FillRect(hdc, &line, brush);
    }

    DeleteObject(brush);
}

/* =========================================================
   PIXELATE
   ========================================================= */

void EffectPixelate(
    HDC hdc,
    int width,
    int height,
    DWORD elapsed
)
{
    int block =
        8 + (int)((elapsed / 100) % 12);

    /*
     * Небольшое усреднение через StretchBlt.
     */

    HDC temp =
        CreateCompatibleDC(hdc);

    if (!temp)
        return;

    HBITMAP bitmap =
        CreateCompatibleBitmap(
            hdc,
            width,
            height
        );

    if (!bitmap)
    {
        DeleteDC(temp);
        return;
    }

    HBITMAP old =
        (HBITMAP)SelectObject(temp, bitmap);

    BitBlt(
        temp,
        0,
        0,
        width,
        height,
        hdc,
        0,
        0,
        SRCCOPY
    );

    for (int y = 0; y < height; y += block)
    {
        for (int x = 0; x < width; x += block)
        {
            int w = block;

            if (x + w > width)
                w = width - x;

            int h = block;

            if (y + h > height)
                h = height - y;

            if (w <= 0 || h <= 0)
                continue;

            StretchBlt(
                hdc,
                x,
                y,
                w,
                h,
                temp,
                x,
                y,
                1,
                1,
                SRCCOPY
            );
        }
    }

    SelectObject(temp, old);

    DeleteObject(bitmap);
    DeleteDC(temp);
}

/* =========================================================
   MIRROR
   ========================================================= */

void EffectMirror(
    HDC hdc,
    int width,
    int height,
    DWORD elapsed
)
{
    (void)elapsed;

    HDC temp =
        CreateCompatibleDC(hdc);

    if (!temp)
        return;

    HBITMAP bitmap =
        CreateCompatibleBitmap(
            hdc,
            width,
            height
        );

    if (!bitmap)
    {
        DeleteDC(temp);
        return;
    }

    HBITMAP old =
        (HBITMAP)SelectObject(temp, bitmap);

    BitBlt(
        temp,
        0,
        0,
        width,
        height,
        hdc,
        0,
        0,
        SRCCOPY
    );

    StretchBlt(
        hdc,
        0,
        0,
        width,
        height,
        temp,
        width,
        0,
        -width,
        height,
        SRCCOPY
    );

    SelectObject(temp, old);

    DeleteObject(bitmap);
    DeleteDC(temp);
}

/* =========================================================
   RGB SPLIT
   ========================================================= */

void EffectRGBSplit(
    HDC hdc,
    int width,
    int height,
    DWORD elapsed
)
{
    int offset =
        (int)((elapsed / 30) % 25) - 12;

    HPEN red =
        CreatePen(
            PS_SOLID,
            2,
            RGB(255, 0, 0)
        );

    HPEN blue =
        CreatePen(
            PS_SOLID,
            2,
            RGB(0, 100, 255)
        );

    if (!red || !blue)
    {
        if (red)
            DeleteObject(red);

        if (blue)
            DeleteObject(blue);

        return;
    }

    HPEN old =
        (HPEN)SelectObject(hdc, red);

    for (int y = 0; y < height; y += 20)
    {
        MoveToEx(hdc, offset, y, NULL);
        LineTo(hdc, width / 2, y);
    }

    SelectObject(hdc, blue);

    for (int y = 10; y < height; y += 20)
    {
        MoveToEx(hdc, width / 2 - offset, y, NULL);
        LineTo(hdc, width, y);
    }

    SelectObject(hdc, old);

    DeleteObject(red);
    DeleteObject(blue);
}