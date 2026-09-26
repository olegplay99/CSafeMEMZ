#include "engine.h"
#include "timeline.h"
#include "../effects/effects.h"

#include <windows.h>

typedef struct
{
    BOOL running;
    DWORD startTime;
    DWORD elapsed;
    TimelineEvent currentEvent;

    int width;
    int height;
} EngineState;

static void EngineInit(EngineState* state)
{
    state->running = TRUE;
    state->startTime = GetTickCount();
    state->elapsed = 0;
    state->currentEvent = EVENT_NONE;

    state->width = GetSystemMetrics(SM_CXSCREEN);
    state->height = GetSystemMetrics(SM_CYSCREEN);

    EffectsInit(NULL);
}

static void EngineUpdate(EngineState* state)
{
    DWORD now = GetTickCount();

    state->elapsed = now - state->startTime;
}

static void EngineProcessEvents(EngineState* state)
{
    TimelineEvent event =
        TimelineGetEvent(state->elapsed);

    if (event != state->currentEvent)
    {
        state->currentEvent = event;
    }

    if (TimelineIsFinished(state->elapsed))
    {
        state->running = FALSE;
    }
}

static void EngineRender(EngineState* state)
{
    HDC hdc = GetDC(NULL);

    if (!hdc)
        return;

    switch (state->currentEvent)
    {
        case EVENT_TEST_1:
            EffectGlitch(
                hdc,
                state->width,
                state->height,
                state->elapsed
            );
            break;

        case EVENT_TEST_2:
            break;

        case EVENT_TEST_3:
            break;

        default:
            break;
    }

    ReleaseDC(NULL, hdc);
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

        Sleep(16);
    }

    return TRUE;
}