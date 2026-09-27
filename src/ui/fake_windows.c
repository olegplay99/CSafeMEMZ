#include "fake_windows.h"
#include "../../include/config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef ARRAYSIZE
#define ARRAYSIZE(a) (sizeof(a) / sizeof((a)[0]))
#endif

typedef enum
{
    FAKE_NOTEPAD = 0,
    FAKE_BROWSER,
    FAKE_ERROR,
    FAKE_CMD,
    FAKE_EXPLORER,
    FAKE_CRASH
} FakeType;

typedef struct
{
    HWND hwnd;
    FakeType type;
    DWORD born;
    DWORD life;
    int vx;
    int vy;
    int wobble;
} FakeWindow;

static HINSTANCE g_instance = NULL;
static FakeWindow g_windows[CS_MEMZ_MAX_WINDOWS];
static int g_count = 0;
static BOOL g_registered = FALSE;
static DWORD g_lastSpawn = 0;
static int g_scriptStep = 0;

static const char* FakeTitle(FakeType type)
{
    switch (type)
    {
        case FAKE_NOTEPAD:  return "Безымянный — Блокнот";
        case FAKE_BROWSER:  return "Chrome — CSafeMEMZ";
        case FAKE_ERROR:    return "CSafeMEMZ — Visual Error";
        case FAKE_CMD:      return "C:\\Windows\\System32\\cmd.exe";
        case FAKE_EXPLORER: return "Проводник — Рабочий стол";
        case FAKE_CRASH:    return "CSafeMEMZ — Explorer stopped responding";
        default:            return "CSafeMEMZ";
    }
}

static void Text(HDC hdc, int x, int y, const char* text, int size, COLORREF color)
{
    HFONT font = CreateFontA(
        size, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH, "Segoe UI");
    if (!font) return;

    {
        HFONT old = (HFONT)SelectObject(hdc, font);
        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, color);
        TextOutA(hdc, x, y, text, (int)strlen(text));
        SelectObject(hdc, old);
    }
    DeleteObject(font);
}

static void Fill(HDC hdc, int l, int t, int r, int b, COLORREF color)
{
    HBRUSH bsh = CreateSolidBrush(color);
    RECT rc = { l, t, r, b };
    if (!bsh) return;
    FillRect(hdc, &rc, bsh);
    DeleteObject(bsh);
}

static FakeType TypeFor(HWND hwnd)
{
    int i;
    for (i = 0; i < g_count; ++i)
        if (g_windows[i].hwnd == hwnd)
            return g_windows[i].type;
    return FAKE_NOTEPAD;
}

static void DrawNotepad(HDC hdc, RECT* r)
{
    Fill(hdc, 0, 0, r->right, r->bottom, RGB(250,250,250));
    Fill(hdc, 0, 0, r->right, 34, RGB(238,238,238));
    Text(hdc, 12, 8, "File    Edit    Format    View    Help", 13, RGB(30,30,30));
    Text(hdc, 16, 55, "CSafeMEMZ.txt", 18, RGB(20,20,20));
    Text(hdc, 16, 90, "something is happening...", 16, RGB(35,35,35));
    Text(hdc, 16, 120, "desktop renderer: ACTIVE", 15, RGB(35,35,35));
    Text(hdc, 16, 150, "visual simulation: SAFE", 15, RGB(35,35,35));
    Text(hdc, 16, 190, "MEMZ-like sequence detected", 16, RGB(120,20,20));
    Text(hdc, 16, 225, "[this window is simulated]", 14, RGB(90,90,90));
}

static void DrawBrowser(HDC hdc, RECT* r)
{
    Fill(hdc, 0, 0, r->right, r->bottom, RGB(250,250,250));
    Fill(hdc, 0, 0, r->right, 42, RGB(235,235,235));
    Fill(hdc, 16, 9, r->right - 55, 34, RGB(255,255,255));
    Text(hdc, 28, 14, "https://csafememz.local/chaos", 13, RGB(60,60,60));
    Text(hdc, 28, 78, "CSafeMEMZ", 31, RGB(25,25,25));
    Text(hdc, 28, 126, "SYSTEM VISUAL CHAOS", 21, RGB(170,30,30));
    Text(hdc, 28, 168, "The desktop is being visually distorted.", 16, RGB(40,40,40));
    Text(hdc, 28, 198, "No files are modified.", 16, RGB(40,40,40));
    Text(hdc, 28, 228, "No persistence. No boot changes. No malware.", 14, RGB(70,70,70));
}

static void DrawError(HDC hdc, RECT* r)
{
    Fill(hdc, 0, 0, r->right, r->bottom, RGB(250,250,250));
    Fill(hdc, 0, 0, r->right, 40, RGB(240,240,240));
    Text(hdc, 18, 11, "CSafeMEMZ", 15, RGB(25,25,25));
    Text(hdc, 26, 72, "Visual simulation warning", 22, RGB(25,25,25));
    Text(hdc, 26, 116, "The desktop has entered a chaotic state.", 16, RGB(40,40,40));
    Text(hdc, 26, 146, "This is not a real system error.", 16, RGB(120,30,30));
    Fill(hdc, r->right - 105, r->bottom - 52, r->right - 18, r->bottom - 18, RGB(230,230,230));
    Text(hdc, r->right - 77, r->bottom - 44, "OK", 14, RGB(25,25,25));
}

static void DrawCMD(HDC hdc, RECT* r)
{
    Fill(hdc, 0, 0, r->right, r->bottom, RGB(10,10,10));
    Text(hdc, 12, 12, "Microsoft Windows [Version 10.0]", 15, RGB(220,220,220));
    Text(hdc, 12, 42, "C:\\> csafememz.exe --visual", 15, RGB(220,220,220));
    Text(hdc, 12, 72, "[OK] desktop renderer initialized", 15, RGB(120,255,120));
    Text(hdc, 12, 102, "[OK] safety mode enabled", 15, RGB(120,255,120));
    Text(hdc, 12, 132, "[!!] chaos level rising...", 15, RGB(255,220,80));
    Text(hdc, 12, 174, "C:\\> _", 15, RGB(220,220,220));
}

static void DrawExplorer(HDC hdc, RECT* r)
{
    Fill(hdc, 0, 0, r->right, r->bottom, RGB(248,248,248));
    Fill(hdc, 0, 0, r->right, 44, RGB(235,235,235));
    Text(hdc, 15, 13, "Проводник", 16, RGB(25,25,25));
    Text(hdc, 18, 64, "Рабочий стол", 20, RGB(25,25,25));
    Text(hdc, 28, 110, "[DIR]  CSafeMEMZ", 17, RGB(35,35,35));
    Text(hdc, 28, 145, "[FILE] README.md", 17, RGB(35,35,35));
    Text(hdc, 28, 180, "[!!]  chaos_mode.exe", 17, RGB(130,30,30));
    Text(hdc, 28, 215, "[simulation only]", 14, RGB(90,90,90));
}

static void DrawCrash(HDC hdc, RECT* r)
{
    Fill(hdc, 0, 0, r->right, r->bottom, RGB(248,248,248));
    Text(hdc, 24, 25, "Explorer.exe", 24, RGB(20,20,20));
    Text(hdc, 24, 70, "This is a simulated crash dialog.", 17, RGB(45,45,45));
    Text(hdc, 24, 108, "CSafeMEMZ is not stopping Windows.", 15, RGB(80,80,80));
    Text(hdc, 24, 140, "The visual sequence will continue.", 15, RGB(80,80,80));
    Fill(hdc, r->right - 130, r->bottom - 52, r->right - 20, r->bottom - 18, RGB(228,228,228));
    Text(hdc, r->right - 101, r->bottom - 43, "Close", 14, RGB(25,25,25));
}

static LRESULT CALLBACK FakeProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    (void)wp; (void)lp;

    switch (msg)
    {
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            RECT r;
            GetClientRect(hwnd, &r);

            switch (TypeFor(hwnd))
            {
                case FAKE_NOTEPAD:  DrawNotepad(hdc, &r);  break;
                case FAKE_BROWSER:  DrawBrowser(hdc, &r);  break;
                case FAKE_ERROR:    DrawError(hdc, &r);    break;
                case FAKE_CMD:      DrawCMD(hdc, &r);      break;
                case FAKE_EXPLORER: DrawExplorer(hdc, &r); break;
                case FAKE_CRASH:    DrawCrash(hdc, &r);    break;
            }

            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_CLOSE:
            DestroyWindow(hwnd);
            return 0;
        case WM_NCHITTEST:
            return HTTRANSPARENT;
        default:
            return DefWindowProcA(hwnd, msg, wp, lp);
    }
}

void FakeWindowsInit(HINSTANCE instance)
{
    WNDCLASSA wc;
    g_instance = instance;
    if (g_registered) return;

    ZeroMemory(&wc, sizeof(wc));
    wc.lpfnWndProc = FakeProc;
    wc.hInstance = g_instance;
    wc.lpszClassName = "CSafeMEMZFakeWindow";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    if (RegisterClassA(&wc) || GetLastError() == ERROR_CLASS_ALREADY_EXISTS)
        g_registered = TRUE;
}

static void RemoveIndex(int index)
{
    int i;
    if (index < 0 || index >= g_count) return;
    for (i = index; i < g_count - 1; ++i)
        g_windows[i] = g_windows[i + 1];
    --g_count;
}

static void Spawn(FakeType type, DWORD now)
{
#if CS_MEMZ_ENABLE_FAKE_WINDOWS
    int sw, sh, w, h, x, y;
    DWORD style;
    HWND hwnd;

    if (!g_registered || g_count >= CS_MEMZ_MAX_WINDOWS) return;

    sw = GetSystemMetrics(SM_CXSCREEN);
    sh = GetSystemMetrics(SM_CYSCREEN);
    w = 430 + rand() % 360;
    h = 250 + rand() % 220;
    if (w > sw - 40) w = sw - 40;
    if (h > sh - 100) h = sh - 100;
    if (w < 300 || h < 200) return;

    x = 20 + rand() % ((sw - w > 40) ? sw - w - 20 : 1);
    y = 40 + rand() % ((sh - h > 80) ? sh - h - 40 : 1);

    style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
    hwnd = CreateWindowExA(
        WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE,
        "CSafeMEMZFakeWindow", FakeTitle(type), style,
        x, y, w, h, NULL, NULL, g_instance, NULL);

    if (!hwnd)
    {
        /* Try a plain top-level window as a fallback. */
        hwnd = CreateWindowExA(
            WS_EX_TOPMOST | WS_EX_APPWINDOW,
            "CSafeMEMZFakeWindow", FakeTitle(type), style,
            x, y, w, h, NULL, NULL, g_instance, NULL);
    }

    if (!hwnd) return;

    g_windows[g_count].hwnd = hwnd;
    g_windows[g_count].type = type;
    g_windows[g_count].born = now;
    g_windows[g_count].life = CS_MEMZ_WINDOW_MIN_LIFE +
        (rand() % (CS_MEMZ_WINDOW_MAX_LIFE - CS_MEMZ_WINDOW_MIN_LIFE + 1));
    g_windows[g_count].vx = (rand() % 3) - 1;
    g_windows[g_count].vy = (rand() % 3) - 1;
    g_windows[g_count].wobble = rand() % 1000;
    ++g_count;

    ShowWindow(hwnd, SW_SHOWNOACTIVATE);
    UpdateWindow(hwnd);
    SetWindowPos(hwnd, HWND_TOPMOST, x, y, w, h,
                 SWP_NOACTIVATE | SWP_SHOWWINDOW);
#else
    (void)type; (void)now;
#endif
}

void FakeWindowsSpawnNotepad(void)  { Spawn(FAKE_NOTEPAD, GetTickCount()); }
void FakeWindowsSpawnBrowser(void)  { Spawn(FAKE_BROWSER, GetTickCount()); }
void FakeWindowsSpawnError(void)    { Spawn(FAKE_ERROR, GetTickCount()); }
void FakeWindowsSpawnCMD(void)      { Spawn(FAKE_CMD, GetTickCount()); }
void FakeWindowsSpawnExplorer(void) { Spawn(FAKE_EXPLORER, GetTickCount()); }
void FakeWindowsSpawnCrash(void)    { Spawn(FAKE_CRASH, GetTickCount()); }

void FakeWindowsUpdate(DWORD elapsed)
{
#if CS_MEMZ_ENABLE_FAKE_WINDOWS
    int i;
    int sw = GetSystemMetrics(SM_CXSCREEN);
    int sh = GetSystemMetrics(SM_CYSCREEN);

    /* Deterministic scripted spawns: these are independent of the visual effect timeline. */
    if (elapsed >= 10000 && g_scriptStep == 0)
    {
        FakeWindowsSpawnNotepad();
        g_scriptStep = 1;
        g_lastSpawn = elapsed;
    }
    else if (elapsed >= 16000 && g_scriptStep == 1)
    {
        FakeWindowsSpawnBrowser();
        g_scriptStep = 2;
        g_lastSpawn = elapsed;
    }
    else if (elapsed >= 22000 && g_scriptStep == 2)
    {
        FakeWindowsSpawnError();
        g_scriptStep = 3;
        g_lastSpawn = elapsed;
    }
    else if (elapsed >= 28000 && g_scriptStep == 3)
    {
        FakeWindowsSpawnCMD();
        g_scriptStep = 4;
        g_lastSpawn = elapsed;
    }
    else if (elapsed >= 34000 && g_scriptStep == 4)
    {
        FakeWindowsSpawnExplorer();
        g_scriptStep = 5;
        g_lastSpawn = elapsed;
    }
    else if (elapsed >= 40000 && g_scriptStep == 5)
    {
        FakeWindowsSpawnCrash();
        g_scriptStep = 6;
        g_lastSpawn = elapsed;
    }
    else if (elapsed >= 46000 && elapsed - g_lastSpawn >= CS_MEMZ_WINDOW_SPAWN_INTERVAL)
    {
        int type = rand() % 6;
        switch (type)
        {
            case 0: FakeWindowsSpawnNotepad();  break;
            case 1: FakeWindowsSpawnBrowser();  break;
            case 2: FakeWindowsSpawnError();    break;
            case 3: FakeWindowsSpawnCMD();      break;
            case 4: FakeWindowsSpawnExplorer(); break;
            default:FakeWindowsSpawnCrash();    break;
        }
        g_lastSpawn = elapsed;
    }

    for (i = 0; i < g_count; ++i)
    {
        HWND hwnd = g_windows[i].hwnd;
        RECT r;
        int width, height, x, y;

        if (!IsWindow(hwnd))
        {
            RemoveIndex(i--);
            continue;
        }

        if (elapsed - g_windows[i].born > g_windows[i].life && elapsed < 90000)
        {
            DestroyWindow(hwnd);
            RemoveIndex(i--);
            continue;
        }

        GetWindowRect(hwnd, &r);
        width = r.right - r.left;
        height = r.bottom - r.top;
        x = r.left;
        y = r.top;

        if (elapsed >= 68000)
        {
            g_windows[i].vx = ((int)((elapsed / 80 + g_windows[i].wobble) % 5)) - 2;
            g_windows[i].vy = ((int)((elapsed / 97 + g_windows[i].wobble) % 5)) - 2;
        }

        x += g_windows[i].vx;
        y += g_windows[i].vy;

        if (x < 0 || x + width > sw) g_windows[i].vx = -g_windows[i].vx;
        if (y < 0 || y + height > sh) g_windows[i].vy = -g_windows[i].vy;

        x = r.left + g_windows[i].vx;
        y = r.top + g_windows[i].vy;
        if (x < 0) x = 0;
        if (y < 0) y = 0;
        if (x + width > sw) x = sw - width;
        if (y + height > sh) y = sh - height;

        ShowWindow(hwnd, SW_SHOWNOACTIVATE);
        SetWindowPos(hwnd, HWND_TOPMOST, x, y, 0, 0,
                     SWP_NOSIZE | SWP_NOACTIVATE | SWP_SHOWWINDOW);
        InvalidateRect(hwnd, NULL, FALSE);
        UpdateWindow(hwnd);
    }
#else
    (void)elapsed;
#endif
}

void FakeWindowsRepaintAll(void)
{
    int i;
    for (i = 0; i < g_count; ++i)
    {
        if (IsWindow(g_windows[i].hwnd))
        {
            InvalidateRect(g_windows[i].hwnd, NULL, FALSE);
            UpdateWindow(g_windows[i].hwnd);
            SetWindowPos(g_windows[i].hwnd, HWND_TOPMOST, 0, 0, 0, 0,
                         SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_SHOWWINDOW);
        }
    }
}

void FakeWindowsShutdown(void)
{
    int i;
    for (i = 0; i < g_count; ++i)
        if (IsWindow(g_windows[i].hwnd)) DestroyWindow(g_windows[i].hwnd);
    g_count = 0;
    g_lastSpawn = 0;
    g_scriptStep = 0;
}
