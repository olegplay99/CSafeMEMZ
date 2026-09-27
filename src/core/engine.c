#include "engine.h"
#include "timeline.h"
#include "event.h"
#include "../effects/effects.h"

#include <windows.h>

typedef struct
{
    BOOL running;

    DWORD startTime;
    DWORD elapsed;

    EventType currentEvent;

    int width;
    int height;

    HWND overlay;

    HDC screenDC;
    HDC backDC;

    HBITMAP backBitmap;
    HBITMAP oldBitmap;

} EngineState;

static EngineState* g_state = NULL;


/* =========================================================
   WINDOW PROC
   ========================================================= */

static LRESULT CALLBACK OverlayProc(
    HWND hwnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam
)
{
    (void)wParam;
    (void)lParam;

    switch (message)
    {
        case WM_ERASEBKGND:
            return 1;

        case WM_PAINT:
        {
            PAINTSTRUCT ps;

            HDC hdc =
                BeginPaint(hwnd, &ps);

            if (g_state && g_state->backDC)
            {
                BitBlt(
                    hdc,
                    0,
                    0,
                    g_state->width,
                    g_state->height,
                    g_state->backDC,
                    0,
                    0,
                    SRCCOPY
                );
            }

            EndPaint(hwnd, &ps);

            return 0;
        }

        case WM_NCHITTEST:
            return HTTRANSPARENT;

        default:
            return DefWindowProc(
                hwnd,
                message,
                wParam,
                lParam
            );
    }
}


/* =========================================================
   OVERLAY CREATE
   ========================================================= */

static BOOL CreateOverlay(
    EngineState* state,
    HINSTANCE hInstance
)
{
    const char* className =
        "CSafeMEMZOverlay";

    WNDCLASSA wc;

    ZeroMemory(
        &wc,
        sizeof(wc)
    );

    wc.lpfnWndProc =
        OverlayProc;

    wc.hInstance =
        hInstance;

    wc.lpszClassName =
        className;

    wc.hCursor =
        LoadCursor(
            NULL,
            IDC_ARROW
        );

    wc.hbrBackground =
        NULL;

    if (!RegisterClassA(&wc))
    {
        if (GetLastError() != ERROR_CLASS_ALREADY_EXISTS)
            return FALSE;
    }

    state->overlay =
        CreateWindowExA(
            WS_EX_TOPMOST |
            WS_EX_TOOLWINDOW |
            WS_EX_NOACTIVATE,

            className,
            "CSafeMEMZ",

            WS_POPUP,

            0,
            0,
            state->width,
            state->height,

            NULL,
            NULL,
            hInstance,
            NULL
        );

    if (!state->overlay)
        return FALSE;

    ShowWindow(
        state->overlay,
        SW_SHOWNOACTIVATE
    );

    UpdateWindow(
        state->overlay
    );

    return TRUE;
}


/* =========================================================
   BACKBUFFER
   ========================================================= */

static BOOL CreateBackbuffer(
    EngineState* state
)
{
    state->screenDC =
        GetDC(NULL);

    if (!state->screenDC)
        return FALSE;

    state->backDC =
        CreateCompatibleDC(
            state->screenDC
        );

    if (!state->backDC)
        return FALSE;

    state->backBitmap =
        CreateCompatibleBitmap(
            state->screenDC,
            state->width,
            state->height
        );

    if (!state->backBitmap)
        return FALSE;

    state->oldBitmap =
        (HBITMAP)SelectObject(
            state->backDC,
            state->backBitmap
        );

    return TRUE;
}


/* =========================================================
   DESTROY BACKBUFFER
   ========================================================= */

static void DestroyBackbuffer(
    EngineState* state
)
{
    if (state->backDC)
    {
        if (state->oldBitmap)
        {
            SelectObject(
                state->backDC,
                state->oldBitmap
            );
        }

        DeleteDC(
            state->backDC
        );

        state->backDC = NULL;
    }

    if (state->backBitmap)
    {
        DeleteObject(
            state->backBitmap
        );

        state->backBitmap = NULL;
    }

    if (state->screenDC)
    {
        ReleaseDC(
            NULL,
            state->screenDC
        );

        state->screenDC = NULL;
    }
}


/* =========================================================
   INIT
   ========================================================= */

static BOOL EngineInit(
    EngineState* state,
    HINSTANCE hInstance
)
{
    ZeroMemory(
        state,
        sizeof(*state)
    );

    state->running = TRUE;

    state->startTime =
        GetTickCount();

    state->elapsed = 0;

    state->currentEvent =
        EVENT_NONE;

    state->width =
        GetSystemMetrics(
            SM_CXSCREEN
        );

    state->height =
        GetSystemMetrics(
            SM_CYSCREEN
        );

    EffectsInit(NULL);

    if (!CreateBackbuffer(state))
        return FALSE;

    if (!CreateOverlay(
            state,
            hInstance))
    {
        DestroyBackbuffer(state);
        return FALSE;
    }

    g_state = state;

    return TRUE;
}


/* =========================================================
   UPDATE
   ========================================================= */

static void EngineUpdate(
    EngineState* state
)
{
    state->elapsed =
        GetTickCount() -
        state->startTime;
}


/* =========================================================
   EVENTS
   ========================================================= */

static void EngineProcessEvents(
    EngineState* state
)
{
    EventType event =
        TimelineGetEvent(
            state->elapsed
        );

    if (event != state->currentEvent)
    {
        if (state->currentEvent != EVENT_NONE)
        {
            EventStop(
                state->currentEvent
            );
        }

        state->currentEvent =
            event;

        if (event != EVENT_NONE)
        {
            EventStart(event);
        }
    }

    if (TimelineIsFinished(
            state->elapsed))
    {
        state->running = FALSE;
    }
}


/* =========================================================
   RENDER
   ========================================================= */

static void EngineRender(
    EngineState* state
)
{
    /*
     * Сначала получаем свежий снимок
     * рабочего стола.
     */

    BitBlt(
        state->backDC,
        0,
        0,
        state->width,
        state->height,
        state->screenDC,
        0,
        0,
        SRCCOPY
    );

    /*
     * Потом применяем эффект
     * непосредственно к backbuffer.
     */

    switch (state->currentEvent)
    {
        case EVENT_GLITCH:

            EffectGlitch(
                state->backDC,
                state->width,
                state->height,
                state->elapsed
            );

            break;

        case EVENT_SHAKE:

            EffectShake(
                state->backDC,
                state->width,
                state->height,
                state->elapsed
            );

            break;

        case EVENT_TEAR:

            EffectTear(
                state->backDC,
                state->width,
                state->height,
                state->elapsed
            );

            break;

        case EVENT_COLOR_SHIFT:

            EffectColorShift(
                state->backDC,
                state->width,
                state->height,
                state->elapsed
            );

            break;

        case EVENT_RECTS:

            EffectRects(
                state->backDC,
                state->width,
                state->height,
                state->elapsed
            );

            break;

        case EVENT_FLASH:

            EffectFlash(
                state->backDC,
                state->width,
                state->height,
                state->elapsed
            );

            break;

        case EVENT_CHAOS:
        {
            DWORD chaosTime = 0;

            if (state->elapsed >= 30000)
                chaosTime =
                    state->elapsed - 30000;

            int intensity =
                (int)(
                    chaosTime * 100 / 10000
                );

            if (intensity < 0)
                intensity = 0;

            if (intensity > 100)
                intensity = 100;

            EffectChaos(
                state->backDC,
                state->width,
                state->height,
                state->elapsed,
                intensity
            );

            break;
        }

        case EVENT_SWIRL:

            EffectSwirl(
                state->backDC,
                state->width,
                state->height,
                state->elapsed
            );

            break;

        case EVENT_SCANLINES:

            EffectScanlines(
                state->backDC,
                state->width,
                state->height,
                state->elapsed
            );

            break;

        case EVENT_PIXELATE:

            EffectPixelate(
                state->backDC,
                state->width,
                state->height,
                state->elapsed
            );

            break;

        case EVENT_MIRROR:

            EffectMirror(
                state->backDC,
                state->width,
                state->height,
                state->elapsed
            );

            break;

        case EVENT_RGB_SPLIT:

            EffectRGBSplit(
                state->backDC,
                state->width,
                state->height,
                state->elapsed
            );

            break;

        case EVENT_NONE:
        default:
            break;
    }

    /*
     * Выводим результат на overlay.
     */

    if (state->overlay)
    {
        InvalidateRect(
            state->overlay,
            NULL,
            FALSE
        );

        UpdateWindow(
            state->overlay
        );
    }
}


/* =========================================================
   SHUTDOWN
   ========================================================= */

static void EngineShutdown(
    EngineState* state
)
{
    if (state->overlay)
    {
        DestroyWindow(
            state->overlay
        );

        state->overlay = NULL;
    }

    DestroyBackbuffer(state);

    g_state = NULL;
}


/* =========================================================
   RUN
   ========================================================= */

BOOL EngineRun(
    HINSTANCE hInstance
)
{
    EngineState state;

    if (!EngineInit(
            &state,
            hInstance))
    {
        return FALSE;
    }

    while (state.running)
    {
        MSG msg;

        while (PeekMessage(
                   &msg,
                   NULL,
                   0,
                   0,
                   PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                state.running = FALSE;
                break;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (!state.running)
            break;

        EngineUpdate(&state);

        EngineProcessEvents(&state);

        EngineRender(&state);

        Sleep(16);
    }

    EngineShutdown(&state);

    return TRUE;
}