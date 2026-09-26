#include "engine.h"
#include "timeline.h"

#include <windows.h>
#include <stdio.h>

typedef struct
{
    BOOL running;
    DWORD startTime;
    DWORD elapsed;
    TimelineEvent currentEvent;
} EngineState;

static void EngineInit(EngineState* state)
{
    state->running = TRUE;
    state->startTime = GetTickCount();
    state->elapsed = 0;
    state->currentEvent = EVENT_NONE;
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

        switch (event)
        {
            case EVENT_TEST_1:
                printf("[ENGINE] Event 1\n");
                break;

            case EVENT_TEST_2:
                printf("[ENGINE] Event 2\n");
                break;

            case EVENT_TEST_3:
                printf("[ENGINE] Event 3\n");
                break;

            case EVENT_NONE:
                printf("[ENGINE] Timeline finished\n");
                break;
        }
    }

    if (TimelineIsFinished(state->elapsed))
    {
        state->running = FALSE;
    }
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

        Sleep(16);
    }

    return TRUE;
}