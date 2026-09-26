#include "effects.h"

#include <stdlib.h>

void EffectsInit(HWND hwnd)
{
    (void)hwnd;
}

void EffectGlitch(
    HDC hdc,
    int width,
    int height,
    DWORD elapsed
)
{
    int offset = (int)((elapsed / 40) % 12);

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