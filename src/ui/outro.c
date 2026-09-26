#include "outro.h"

static LRESULT CALLBACK OutroProc(
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
                L"ОК, пока!",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                135, 210, 180, 40,
                hwnd,
                (HMENU)1,
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
                CreateSolidBrush(RGB(10, 25, 10));

            FillRect(hdc, &rc, background);
            DeleteObject(background);

            SetBkMode(hdc, TRANSPARENT);

            HFONT font = CreateFontW(
                24,
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
                L"Consolas"
            );

            HGDIOBJ oldFont =
                SelectObject(hdc, font);

            SetTextColor(
                hdc,
                RGB(80, 255, 120)
            );

            const wchar_t* title =
                L"> CSafeMEMZ завершён.";

            TextOutW(
                hdc,
                25,
                30,
                title,
                lstrlenW(title)
            );

            SelectObject(hdc, oldFont);

            DeleteObject(font);

            EndPaint(hwnd, &ps);

            return 0;
        }

        case WM_COMMAND:
            if (LOWORD(wParam) == 1)
                DestroyWindow(hwnd);

            return 0;

        case WM_CLOSE:
            DestroyWindow(hwnd);
            return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProcW(
        hwnd,
        msg,
        wParam,
        lParam
    );
}

void ShowOutro(HINSTANCE hInstance)
{
    const wchar_t CLASS_NAME[] =
        L"CSafeMEMZ_Outro";

    WNDCLASSW wc = {0};

    wc.lpfnWndProc = OutroProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClassW(&wc);

    int width = 450;
    int height = 300;

    int screenWidth =
        GetSystemMetrics(SM_CXSCREEN);

    int screenHeight =
        GetSystemMetrics(SM_CYSCREEN);

    HWND hwnd = CreateWindowExW(
        WS_EX_TOPMOST,
        CLASS_NAME,
        L"CSafeMEMZ — завершение",
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
        return;

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    MSG msg;

    while (GetMessageW(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
}