#include "engine.h"
#include "timeline.h"
#include "event.h"
#include "../effects/effects.h"
#include "../ui/fake_windows.h"
#include "../ui/outro.h"
#include "../../include/config.h"

#include <stdlib.h>
#include <time.h>

/*
   Important: there is NO overlay window here.
   The engine captures the desktop, modifies the backbuffer, and writes the
   resulting frame directly to the desktop DC. This is why the effect looks
   like it is happening on the desktop rather than inside a fake fullscreen window.
*/
typedef struct
{
    BOOL running;
    DWORD startTime;
    DWORD elapsed;
    EventType currentEvent;
    int width;
    int height;
    HDC desktopDC;
    HDC backDC;
    HBITMAP backBitmap;
    HBITMAP oldBitmap;
    HINSTANCE instance;
} EngineState;

static EngineState* g_state=NULL;

static BOOL CreateBackbuffer(EngineState* s)
{
    s->desktopDC=GetDC(NULL);
    if(!s->desktopDC)return FALSE;
    s->backDC=CreateCompatibleDC(s->desktopDC);
    if(!s->backDC)return FALSE;
    s->backBitmap=CreateCompatibleBitmap(s->desktopDC,s->width,s->height);
    if(!s->backBitmap)return FALSE;
    s->oldBitmap=(HBITMAP)SelectObject(s->backDC,s->backBitmap);
    return TRUE;
}

static void DestroyBackbuffer(EngineState* s)
{
    if(s->backDC)
    {
        if(s->oldBitmap)SelectObject(s->backDC,s->oldBitmap);
        DeleteDC(s->backDC);s->backDC=NULL;
    }
    if(s->backBitmap){DeleteObject(s->backBitmap);s->backBitmap=NULL;}
    if(s->desktopDC){ReleaseDC(NULL,s->desktopDC);s->desktopDC=NULL;}
}

static BOOL EngineInit(EngineState* s,HINSTANCE instance)
{
    ZeroMemory(s,sizeof(*s));
    s->running=TRUE;
    s->startTime=GetTickCount();
    s->currentEvent=EVENT_NONE;
    s->width=GetSystemMetrics(SM_CXSCREEN);
    s->height=GetSystemMetrics(SM_CYSCREEN);
    s->instance=instance;
    srand((unsigned int)time(NULL));
    if(!CreateBackbuffer(s)){DestroyBackbuffer(s);return FALSE;}
    FakeWindowsInit(instance);
    EffectsInit(NULL);
    g_state=s;
    return TRUE;
}

static void EngineUpdate(EngineState* s)
{
    s->elapsed=GetTickCount()-s->startTime;
    FakeWindowsUpdate(s->elapsed);

    /* Escape is the emergency exit for the visual simulator. */
    if(GetAsyncKeyState(VK_ESCAPE)&0x8000)s->running=FALSE;
}

static void EngineProcessEvents(EngineState* s)
{
    EventType e=TimelineGetEvent(s->elapsed);
    if(e!=s->currentEvent)
    {
        if(s->currentEvent!=EVENT_NONE)EventStop(s->currentEvent);
        s->currentEvent=e;
        if(e!=EVENT_NONE)EventStart(e);
    }
    if(TimelineIsFinished(s->elapsed))s->running=FALSE;
}

static void RenderEvent(EngineState* s)
{
    DWORD t=s->elapsed;
    switch(s->currentEvent)
    {
        case EVENT_GLITCH:          EffectGlitch(s->backDC,s->width,s->height,t); break;
        case EVENT_SHAKE:           EffectShake(s->backDC,s->width,s->height,t); break;
        case EVENT_TEAR:            EffectTear(s->backDC,s->width,s->height,t); break;
        case EVENT_COLOR_SHIFT:     EffectColorShift(s->backDC,s->width,s->height,t); break;
        case EVENT_RECTS:           EffectRects(s->backDC,s->width,s->height,t); break;
        case EVENT_FLASH:           EffectFlash(s->backDC,s->width,s->height,t); break;
        case EVENT_SWIRL:           EffectSwirl(s->backDC,s->width,s->height,t); break;
        case EVENT_SCANLINES:       EffectScanlines(s->backDC,s->width,s->height,t); break;
        case EVENT_PIXELATE:        EffectPixelate(s->backDC,s->width,s->height,t); break;
        case EVENT_MIRROR:          EffectMirror(s->backDC,s->width,s->height,t); break;
        case EVENT_RGB_SPLIT:       EffectRGBSplit(s->backDC,s->width,s->height,t); break;
        case EVENT_WAVE:            EffectWave(s->backDC,s->width,s->height,t); break;
        case EVENT_INVERT:          EffectInvert(s->backDC,s->width,s->height,t); break;
        case EVENT_VERTICAL_TEAR:   EffectVerticalTear(s->backDC,s->width,s->height,t); break;
        case EVENT_BARS:            EffectBars(s->backDC,s->width,s->height,t); break;
        case EVENT_CHECKER:         EffectChecker(s->backDC,s->width,s->height,t); break;
        case EVENT_ZOOM:            EffectZoom(s->backDC,s->width,s->height,t); break;
        case EVENT_NOISE:           EffectNoise(s->backDC,s->width,s->height,t); break;
        case EVENT_WARP:            EffectWarp(s->backDC,s->width,s->height,t); break;
        case EVENT_COLOR_BANDS:     EffectColorBands(s->backDC,s->width,s->height,t); break;
        case EVENT_SPIRAL:          EffectSpiral(s->backDC,s->width,s->height,t); break;
        case EVENT_DESKTOP_FLICKER: EffectFlicker(s->backDC,s->width,s->height,t); break;
        case EVENT_CHAOS:
        {
            DWORD local=t>=85000?t-85000:0;
            int intensity=(int)(local/50);
            if(intensity>100)intensity=100;
            EffectFinalChaos(s->backDC,s->width,s->height,t,intensity);
            break;
        }
        case EVENT_FINAL:
            OutroDraw(s->backDC,s->width,s->height,t);
            break;
        default: break;
    }
}

static void EngineRender(EngineState* s)
{
    /* Fresh desktop snapshot each frame. */
    BitBlt(s->backDC,0,0,s->width,s->height,s->desktopDC,0,0,SRCCOPY);

    /*
       There is deliberately NO fullscreen intro window and NO overlay.
       The first frame is the real desktop, and every visual effect is
       composited back into the desktop DC itself.
    */
    RenderEvent(s);

    /* The actual desktop is the render target. */
    BitBlt(s->desktopDC,0,0,s->width,s->height,s->backDC,0,0,SRCCOPY);

    /* Keep simulated windows visible above the desktop render. */
    FakeWindowsRepaintAll();
}

static void EngineShutdown(EngineState* s)
{
    FakeWindowsShutdown();
    EffectsShutdown();
    DestroyBackbuffer(s);
    g_state=NULL;
}

BOOL EngineRun(HINSTANCE hInstance)
{
    EngineState s;
    MSG msg;

    if(!EngineInit(&s,hInstance))return FALSE;

    while(s.running)
    {
        while(PeekMessageA(&msg,NULL,0,0,PM_REMOVE))
        {
            if(msg.message==WM_QUIT){s.running=FALSE;break;}
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }
        if(!s.running)break;
        EngineUpdate(&s);
        EngineProcessEvents(&s);
        EngineRender(&s);
        Sleep(CS_MEMZ_FPS_DELAY);
    }

    EngineShutdown(&s);
    return TRUE;
}
