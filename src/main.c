#include <windows.h>

#include "ui/intro.h"
#include "ui/outro.h"

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

    /*
        Здесь позже появится основной
        CSafeMEMZ engine.
    */

    ShowOutro(hInstance);

    return 0;
}