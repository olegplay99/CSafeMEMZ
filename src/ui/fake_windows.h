#ifndef CSAFEMEMZ_FAKE_WINDOWS_H
#define CSAFEMEMZ_FAKE_WINDOWS_H

#include <windows.h>

void FakeWindowsInit(HINSTANCE instance);
void FakeWindowsUpdate(DWORD elapsed);
void FakeWindowsRepaintAll(void);
void FakeWindowsSpawnNotepad(void);
void FakeWindowsSpawnBrowser(void);
void FakeWindowsSpawnError(void);
void FakeWindowsSpawnCMD(void);
void FakeWindowsSpawnExplorer(void);
void FakeWindowsSpawnCrash(void);
void FakeWindowsShutdown(void);

#endif
