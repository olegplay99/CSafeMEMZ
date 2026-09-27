#ifndef CSAFEMEMZ_TIMELINE_H
#define CSAFEMEMZ_TIMELINE_H

#include <windows.h>
#include "event.h"

EventType TimelineGetEvent(DWORD elapsed);
BOOL TimelineIsFinished(DWORD elapsed);
DWORD TimelineDuration(void);

#endif
