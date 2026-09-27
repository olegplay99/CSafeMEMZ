#include <windows.h>
#include "core/engine.h"

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
    (void)hPrevInstance;(void)lpCmdLine;(void)nCmdShow;
    return EngineRun(hInstance)?0:1;
}
