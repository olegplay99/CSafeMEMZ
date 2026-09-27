#ifndef CSAFEMEMZ_EVENT_H
#define CSAFEMEMZ_EVENT_H

typedef enum
{
    EVENT_NONE = 0,

    EVENT_GLITCH,
    EVENT_SHAKE,
    EVENT_TEAR,
    EVENT_COLOR_SHIFT,
    EVENT_RECTS,
    EVENT_FLASH,

    EVENT_CHAOS

} EventType;

void EventStart(EventType event);
void EventStop(EventType event);

#endif