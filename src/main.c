#include <windows.h>

#include "ui/intro.h"
#include "ui/outro.h"
#include "core/engine.h"

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow
)
{
    (void)hPrevInstance;
    (void)lpCmdLine;
    (void)nCmdShow;

    if (!ShowIntro(hInstance))
        return 0;

    EngineRun(hInstance);

    ShowOutro(hInstance);

    return 0;
}