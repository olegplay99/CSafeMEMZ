#ifndef CSAFEMEMZ_TIMELINE_H
#define CSAFEMEMZ_TIMELINE_H

#include <windows.h>

typedef enum
{
    EVENT_NONE = 0,
    EVENT_TEST_1,
    EVENT_TEST_2,
    EVENT_TEST_3
} TimelineEvent;

TimelineEvent TimelineGetEvent(DWORD elapsed);

BOOL TimelineIsFinished(DWORD elapsed);

#endif