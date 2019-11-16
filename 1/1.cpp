#ifndef UNICODE
#define UNICODE
#define UNICODE_WAS_UNDEFINED
#endif

#include <Windows.h>
#include <tchar.h>
#include <math.h>

#ifdef UNICODE_WAS_UNDEFINED
#undef UNICODE
#endif

#define IDI_ICON1     101
#define IDC_CURSOR1   102
#define IDR_MENU1     103
#define ID_START      40001
#define ID_STOP       40002
#define ID_TIMER      40003

struct Text {

    bool active = false;
    int angel = 0;
    int shift = 10;
    int x = 330, x_0 = 330;
    int y = 250, y_0 = 250;

    LPCWSTR str = L"BSUIR HELLO";

} text;

void Move(RECT* rect) {

    if(text.active) {

        text.angel += text.shift;
        text.angel %= 360;

        text.x = 100 * cos(text.angel * (3.14 / 180.0)) + text.x_0;
        text.y = 100 * sin(text.angel * (3.14 / 180.0)) + text.y_0;

    }


}

LRESULT CALLBACK WndProc(
    HWND   hwnd,
    UINT   msg,
    WPARAM wParam,
    LPARAM lParam
) {

    switch(msg) {

        case WM_CREATE: {
            if(!SetTimer(hwnd, ID_TIMER, 30, NULL)) {
                MessageBox(hwnd, L"Window registration is failed!", L"Error!", MB_ICONWARNING | MB_OK);
                return false;
            }

            HDC hdc = GetDC(hwnd);
            ReleaseDC(hwnd, hdc);

            break;
        }

        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case ID_START: {
                    text.active = true;
                    break;
                }
                case ID_STOP: {
                    text.active = false;
                    break;
                }
            }

            break;
        }

        case WM_CLOSE: {
            DestroyWindow(hwnd);
            break;
        }

        case WM_DESTROY: {
            KillTimer(hwnd, ID_TIMER);
            PostQuitMessage(0);
            break;
        }

        case WM_TIMER: {
            RECT rect;
            HDC hdc = GetDC(hwnd);
            GetClientRect(hwnd, &rect);

            Move(&rect);

            InvalidateRect(hwnd, NULL, TRUE);
            ReleaseDC(hwnd, hdc);
            break;
        }

        case WM_PAINT: {
            PAINTSTRUCT paintStruct;
            HDC hdc = BeginPaint(hwnd, &paintStruct);

            TextOut(hdc, text.x, text.y, text.str, lstrlen(text.str));
            EndPaint(hwnd, &paintStruct);
            break;
        }

        default: {
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }
    }

}


int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nCmdShow
) {

    MSG        msg;
    HWND       hwnd;
    WNDCLASSEX wc;

    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
    wc.hCursor       = LoadCursor(GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_CURSOR1));
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName  = MAKEINTRESOURCE(IDR_MENU1);
    wc.lpszClassName = L"MagneetWindowClass";
    wc.hIconSm       = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));

    if(!RegisterClassEx(&wc)) {
        MessageBox(NULL, L"Window registration is failed!", L"Error!", MB_ICONWARNING | MB_OK);
        return 0;
    }

    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        L"MagneetWindowClass",
        L"Sys Programming, Lab: 1",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL, NULL, hInstance, NULL
    );

    if(hwnd == NULL) {
        MessageBox(NULL, L"Window creation is failed!", L"Error!", MB_ICONWARNING | MB_OK);
        return 0;
    }

    HMENU hMenu = CreateMenu();
    AppendMenu(hMenu, MF_STRING, ID_START, L"&Start");
    AppendMenu(hMenu, MF_STRING, ID_STOP, L"&Stop");
    SetMenu(hwnd, hMenu);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while(GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}
