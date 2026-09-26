#include "intro.h"

static BOOL g_introResult = FALSE;

static LRESULT CALLBACK IntroProc(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam
)
{
    switch (msg)
    {
        case WM_CREATE:
        {
            CreateWindowW(
                L"BUTTON",
                L"Да",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                60, 250, 130, 40,
                hwnd,
                (HMENU)1,
                NULL,
                NULL
            );

            CreateWindowW(
                L"BUTTON",
                L"Не, не сегодня",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                210, 250, 180, 40,
                hwnd,
                (HMENU)2,
                NULL,
                NULL
            );

            return 0;
        }

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            RECT rc;
            GetClientRect(hwnd, &rc);

            HBRUSH background =
                CreateSolidBrush(RGB(15, 15, 20));

            FillRect(hdc, &rc, background);
            DeleteObject(background);

            SetBkMode(hdc, TRANSPARENT);

            HFONT titleFont = CreateFontW(
                28,
                0,
                0,
                0,
                FW_BOLD,
                FALSE,
                FALSE,
                FALSE,
                DEFAULT_CHARSET,
                OUT_DEFAULT_PRECIS,
                CLIP_DEFAULT_PRECIS,
                CLEARTYPE_QUALITY,
                DEFAULT_PITCH,
                L"Segoe UI"
            );

            HFONT textFont = CreateFontW(
                17,
                0,
                0,
                0,
                FW_NORMAL,
                FALSE,
                FALSE,
                FALSE,
                DEFAULT_CHARSET,
                OUT_DEFAULT_PRECIS,
                CLIP_DEFAULT_PRECIS,
                CLEARTYPE_QUALITY,
                DEFAULT_PITCH,
                L"Segoe UI"
            );

            HGDIOBJ oldFont =
                SelectObject(hdc, titleFont);

            SetTextColor(
                hdc,
                RGB(255, 70, 70)
            );

            const wchar_t* title =
                L"CSafeMEMZ";

            TextOutW(
                hdc,
                25,
                25,
                title,
                lstrlenW(title)
            );

            SelectObject(hdc, textFont);

            SetTextColor(
                hdc,
                RGB(230, 230, 230)
            );

            const wchar_t* lines[] =
            {
                L"Это безопасная визуальная имитация MEMZ.",
                L"",
                L"Программа будет показывать различные",
                L"визуальные эффекты и фейковые системные события.",
                L"",
                L"Никакие файлы, загрузчик или MBR не изменяются.",
                L"",
                L"Продолжить?"
            };

            int y = 80;

            for (int i = 0; i < 8; ++i)
            {
                TextOutW(
                    hdc,
                    25,
                    y,
                    lines[i],
                    lstrlenW(lines[i])
                );

                y += 24;
            }

            SelectObject(hdc, oldFont);

            DeleteObject(titleFont);
            DeleteObject(textFont);

            EndPaint(hwnd, &ps);

            return 0;
        }

        case WM_COMMAND:
        {
            switch (LOWORD(wParam))
            {
                case 1:
                    g_introResult = TRUE;
                    DestroyWindow(hwnd);
                    return 0;

                case 2:
                    g_introResult = FALSE;
                    DestroyWindow(hwnd);
                    return 0;
            }

            return 0;
        }

        case WM_CLOSE:
        {
            g_introResult = FALSE;
            DestroyWindow(hwnd);
            return 0;
        }

        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }
    }

    return DefWindowProcW(
        hwnd,
        msg,
        wParam,
        lParam
    );
}

BOOL ShowIntro(HINSTANCE hInstance)
{
    const wchar_t CLASS_NAME[] =
        L"CSafeMEMZ_Intro";

    WNDCLASSW wc = {0};

    wc.lpfnWndProc = IntroProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClassW(&wc);

    int width = 470;
    int height = 350;

    int screenWidth =
        GetSystemMetrics(SM_CXSCREEN);

    int screenHeight =
        GetSystemMetrics(SM_CYSCREEN);

    HWND hwnd = CreateWindowExW(
        WS_EX_TOPMOST,
        CLASS_NAME,
        L"CSafeMEMZ",
        WS_OVERLAPPED |
        WS_CAPTION |
        WS_SYSMENU,
        (screenWidth - width) / 2,
        (screenHeight - height) / 2,
        width,
        height,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!hwnd)
        return FALSE;

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    MSG msg;

    while (GetMessageW(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return g_introResult;
}