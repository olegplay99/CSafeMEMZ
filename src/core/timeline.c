#include "timeline.h"
#include "../../include/config.h"

typedef struct
{
    DWORD start;
    DWORD end;
    EventType event;
} TimelineEntry;

/* Deliberately scripted: the simulator grows from subtle glitches into a final visual storm. */
static const TimelineEntry g_timeline[] =
{
    {     0,  5000, EVENT_NONE },
    {  5000,  8500, EVENT_GLITCH },
    {  8500, 12000, EVENT_SHAKE },
    { 12000, 15500, EVENT_TEAR },
    { 15500, 18500, EVENT_COLOR_SHIFT },
    { 18500, 21500, EVENT_SWIRL },
    { 21500, 24500, EVENT_WAVE },
    { 24500, 28000, EVENT_FAKE_NOTEPAD },
    { 28000, 31500, EVENT_RGB_SPLIT },
    { 31500, 35000, EVENT_FAKE_BROWSER },
    { 35000, 38000, EVENT_PIXELATE },
    { 38000, 41000, EVENT_FAKE_ERROR },
    { 41000, 44000, EVENT_VERTICAL_TEAR },
    { 44000, 47000, EVENT_FAKE_CMD },
    { 47000, 50000, EVENT_NOISE },
    { 50000, 53000, EVENT_FAKE_EXPLORER },
    { 53000, 56000, EVENT_WARP },
    { 56000, 59000, EVENT_MIRROR },
    { 59000, 62000, EVENT_INVERT },
    { 62000, 65000, EVENT_COLOR_BANDS },
    { 65000, 68000, EVENT_SPIRAL },
    { 68000, 71000, EVENT_CHECKER },
    { 71000, 74000, EVENT_ZOOM },
    { 74000, 77000, EVENT_BARS },
    { 77000, 80000, EVENT_DESKTOP_FLICKER },
    { 80000, 85000, EVENT_FAKE_CRASH },
    { 85000, 90000, EVENT_CHAOS },
    { 90000, CS_MEMZ_DURATION, EVENT_FINAL }
};

#define TIMELINE_COUNT ((unsigned int)(sizeof(g_timeline) / sizeof(g_timeline[0])))

EventType TimelineGetEvent(DWORD elapsed)
{
    unsigned int i;
    for (i = 0; i < TIMELINE_COUNT; ++i)
    {
        if (elapsed >= g_timeline[i].start && elapsed < g_timeline[i].end)
            return g_timeline[i].event;
    }
    return EVENT_FINAL;
}

BOOL TimelineIsFinished(DWORD elapsed)
{
    return elapsed >= CS_MEMZ_DURATION;
}

DWORD TimelineDuration(void)
{
    return CS_MEMZ_DURATION;
}
