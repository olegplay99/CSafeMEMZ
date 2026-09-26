#ifndef CSAFEMEMZ_EVENT_H
#define CSAFEMEMZ_EVENT_H

#include <windows.h>

typedef enum
{
    EVENT_NONE = 0,

    EVENT_GLITCH,
    EVENT_INVERT,
    EVENT_FAKE_DIALOG

} EventType;

void EventStart(EventType event);
void EventUpdate(EventType event, DWORD elapsed);
void EventStop(EventType event);

#endif