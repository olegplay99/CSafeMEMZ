#include "timeline.h"

TimelineEvent TimelineGetEvent(DWORD elapsed)
{
    if (elapsed < 5000)
        return EVENT_TEST_1;

    if (elapsed < 10000)
        return EVENT_TEST_2;

    if (elapsed < 15000)
        return EVENT_TEST_3;

    return EVENT_NONE;
}

BOOL TimelineIsFinished(DWORD elapsed)
{
    return elapsed >= 15000;
}