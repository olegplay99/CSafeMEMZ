#include "event.h"

#include <stdio.h>

void EventStart(EventType event)
{
    switch (event)
    {
        case EVENT_GLITCH:
            printf("[EVENT] GLITCH START\n");
            break;

        case EVENT_INVERT:
            printf("[EVENT] INVERT START\n");
            break;

        case EVENT_FAKE_DIALOG:
            printf("[EVENT] FAKE DIALOG START\n");
            break;

        default:
            break;
    }
}

void EventUpdate(EventType event, DWORD elapsed)
{
    (void)elapsed;

    switch (event)
    {
        case EVENT_GLITCH:
            break;

        case EVENT_INVERT:
            break;

        case EVENT_FAKE_DIALOG:
            break;

        default:
            break;
    }
}

void EventStop(EventType event)
{
    switch (event)
    {
        case EVENT_GLITCH:
            printf("[EVENT] GLITCH STOP\n");
            break;

        case EVENT_INVERT:
            printf("[EVENT] INVERT STOP\n");
            break;

        case EVENT_FAKE_DIALOG:
            printf("[EVENT] FAKE DIALOG STOP\n");
            break;

        default:
            break;
    }
}