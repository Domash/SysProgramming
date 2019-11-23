#ifndef UNICODE
#define UNICODE
#define UNICODE_WAS_UNDEFINED
#endif

#include <Windows.h>

#ifdef UNICODE_WAS_UNDEFINED
#undef UNICODE
#endif

HWND hWND = NULL;
HANDLE mutex = NULL;

int LIMIT = 5000;

#define GENERATE_DRAW_FUNCTION(name, brush)                     \
    DWORD WINAPI draw_func_##name(LPVOID lpParam) {             \
        while(LIMIT > 0) {                                      \
            DWORD res = WaitForSingleObject(mutex, INFINITE);   \
            if(res == WAIT_OBJECT_0) {                          \
                RECT rect;                                      \
                GetClientRect(hWND, &rect);                     \
                HDC hdc = GetDC(hWND);                          \
                SelectObject(hdc, brush);                       \
                FillRect(hdc, &rect, brush);                    \
                LIMIT--;                                        \
                ReleaseMutex(mutex);                            \
                Sleep(900);                                     \
            }                                                   \
        }                                                       \
        ExitThread(0);                                          \
    }

HBRUSH brushes[3] = {
    CreateSolidBrush(RGB(0, 0, 255)),
    CreateSolidBrush(RGB(0, 255, 0)),
    CreateSolidBrush(RGB(255, 0, 0))
};

GENERATE_DRAW_FUNCTION(R, brushes[0]);
GENERATE_DRAW_FUNCTION(G, brushes[1]);
GENERATE_DRAW_FUNCTION(B, brushes[2]);

HANDLE threads[3] = {0};

LRESULT CALLBACK WndProc(
    HWND   hwnd,
    UINT   msg,
    WPARAM wParam,
    LPARAM lParam
) {

    switch(msg) {

        case WM_CREATE: {

            mutex = CreateMutex(NULL, FALSE, L"DomashMutex");
            threads[0] = CreateThread(NULL, NULL, draw_func_R, NULL, NULL, NULL);
            threads[1] = CreateThread(NULL, NULL, draw_func_G, NULL, NULL, NULL);
            threads[2] = CreateThread(NULL, NULL, draw_func_B, NULL, NULL, NULL);

            break;
        }

        case WM_GETMINMAXINFO: {

            MINMAXINFO* mmi = (MINMAXINFO*)lParam;
            mmi -> ptMinTrackSize.x = 1000;
            mmi -> ptMinTrackSize.y = 500;
            mmi -> ptMaxTrackSize.x = 1000;
            mmi -> ptMaxTrackSize.y = 500;

            break;
        }

        case WM_CLOSE: {

            for(size_t i = 0; i < 3; ++i) {
                TerminateThread(threads[i], 0);
            }

            WaitForMultipleObjects(3, threads, TRUE, INFINITE);

            for(size_t i = 0; i < 3; ++i) {
                if(threads[i] != INVALID_HANDLE_VALUE) {
                    CloseHandle(threads[i]);
                }
            }

            CloseHandle(mutex);
            DestroyWindow(hwnd);

            ExitProcess(0);

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
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = L"MagneetWindowClass";
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc)) {
        MessageBox(NULL, L"Window registration is failed!", L"Error!", MB_ICONWARNING | MB_OK);
        return 0;
    }

    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        L"MagneetWindowClass",
        L"Sys Programming, Lab: 7",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL, NULL, hInstance, NULL
    );

    if(hwnd == NULL) {
        MessageBox(NULL, L"Window creation is failed!", L"Error!", MB_ICONWARNING | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    hWND = hwnd;

    while(GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

