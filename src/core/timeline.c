#include "timeline.h"

EventType TimelineGetEvent(DWORD elapsed)
{
if (elapsed < 5000)
return EVENT_GLITCH;

if (elapsed < 10000)
    return EVENT_SHAKE;

if (elapsed < 15000)
    return EVENT_TEAR;

if (elapsed < 20000)
    return EVENT_COLOR_SHIFT;

if (elapsed < 25000)
    return EVENT_RECTS;

if (elapsed < 30000)
    return EVENT_FLASH;

if (elapsed < 40000)
    return EVENT_CHAOS;

if (elapsed < 45000)
    return EVENT_SWIRL;

if (elapsed < 50000)
    return EVENT_SCANLINES;

if (elapsed < 55000)
    return EVENT_PIXELATE;

if (elapsed < 60000)
    return EVENT_MIRROR;

if (elapsed < 65000)
    return EVENT_RGB_SPLIT;

return EVENT_NONE;


}

BOOL TimelineIsFinished(DWORD elapsed)
{
return elapsed >= 65000;
}
