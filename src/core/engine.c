#include "engine.h"
#include "timeline.h"
#include "event.h"
#include "../effects/effects.h"

#include <windows.h>

typedef struct
{
    BOOL running;

    DWORD startTime;
    DWORD elapsed;

    EventType currentEvent;

    int width;
    int height;

} EngineState;


static void EngineInit(EngineState* state)
{
    state->running = TRUE;

    state->startTime = GetTickCount();
    state->elapsed = 0;

    state->currentEvent = EVENT_NONE;

    state->width =
        GetSystemMetrics(SM_CXSCREEN);

    state->height =
        GetSystemMetrics(SM_CYSCREEN);

    EffectsInit(NULL);
}


static void EngineUpdate(EngineState* state)
{
    DWORD now = GetTickCount();

    state->elapsed =
        now - state->startTime;
}


static void EngineProcessEvents(
    EngineState* state
)
{
    EventType event =
        TimelineGetEvent(state->elapsed);

    if (event != state->currentEvent)
    {
        if (state->currentEvent != EVENT_NONE)
        {
            EventStop(
                state->currentEvent
            );
        }

        state->currentEvent = event;

        if (event != EVENT_NONE)
        {
            EventStart(event);
        }
    }

    if (TimelineIsFinished(state->elapsed))
    {
        state->running = FALSE;
    }
}


static void EngineRender(
    EngineState* state
)
{
    HDC hdc =
        GetDC(NULL);

    if (!hdc)
        return;


    switch (state->currentEvent)
    {
        case EVENT_GLITCH:

            EffectGlitch(
                hdc,
                state->width,
                state->height,
                state->elapsed
            );

            break;


        case EVENT_SHAKE:

            EffectShake(
                hdc,
                state->width,
                state->height,
                state->elapsed
            );

            break;


        case EVENT_TEAR:

            EffectTear(
                hdc,
                state->width,
                state->height,
                state->elapsed
            );

            break;


        case EVENT_COLOR_SHIFT:

            EffectColorShift(
                hdc,
                state->width,
                state->height,
                state->elapsed
            );

            break;


        case EVENT_RECTS:

            EffectRects(
                hdc,
                state->width,
                state->height,
                state->elapsed
            );

            break;


        case EVENT_FLASH:

            EffectFlash(
                hdc,
                state->width,
                state->height,
                state->elapsed
            );

            break;


        case EVENT_CHAOS:

            /*
             * Финальный комбо-режим.
             *
             * Все основные эффекты работают
             * одновременно.
             */

            EffectGlitch(
                hdc,
                state->width,
                state->height,
                state->elapsed
            );

            EffectShake(
                hdc,
                state->width,
                state->height,
                state->elapsed
            );

            EffectTear(
                hdc,
                state->width,
                state->height,
                state->elapsed
            );

            EffectColorShift(
                hdc,
                state->width,
                state->height,
                state->elapsed
            );

            EffectRects(
                hdc,
                state->width,
                state->height,
                state->elapsed
            );

            break;


        case EVENT_NONE:
        default:

            break;
    }


    ReleaseDC(
        NULL,
        hdc
    );
}


BOOL EngineRun(HINSTANCE hInstance)
{
    (void)hInstance;

    EngineState state;

    EngineInit(&state);


    while (state.running)
    {
        EngineUpdate(&state);

        EngineProcessEvents(&state);

        EngineRender(&state);

        /*
         * ~60 FPS
         */

        Sleep(16);
    }


    return TRUE;
}