#include "event.h"
#include "../ui/fake_windows.h"

void EventStart(EventType event)
{
    switch (event)
    {
        case EVENT_FAKE_NOTEPAD:        FakeWindowsSpawnNotepad();  break;
        case EVENT_FAKE_BROWSER:        FakeWindowsSpawnBrowser();  break;
        case EVENT_FAKE_ERROR:          FakeWindowsSpawnError();    break;
        case EVENT_FAKE_CMD:            FakeWindowsSpawnCMD();      break;
        case EVENT_FAKE_EXPLORER:       FakeWindowsSpawnExplorer();break;
        case EVENT_FAKE_CRASH:          FakeWindowsSpawnCrash();   break;
        default: break;
    }
}

void EventStop(EventType event)
{
    (void)event;
}
