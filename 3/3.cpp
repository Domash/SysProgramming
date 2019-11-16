#ifndef UNICODE
#define UNICODE
#define UNICODE_WAS_UNDEFINED
#endif

#include <Windows.h>

#ifdef UNICODE_WAS_UNDEFINED
#undef UNICODE
#endif

#define ID_BUTTON_DRAW  101
#define ID_BUTTON_CLEAR 102

LRESULT CALLBACK WndProc(
    HWND   hwnd,
    UINT   msg,
    WPARAM wParam,
    LPARAM lParam
) {

    switch(msg) {

        case WM_CREATE: {

            HWND draw_button  = CreateWindow(L"Button", L"Draw", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
                                             290, 500, 100, 50, hwnd, (HMENU)ID_BUTTON_DRAW, NULL, NULL);
            HWND clear_button = CreateWindow(L"Button", L"Clear", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
                                             410, 500, 100, 50, hwnd, (HMENU)ID_BUTTON_CLEAR, NULL, NULL);

            break;
        }

        case WM_DRAWITEM: {

            LPDRAWITEMSTRUCT pdis = (LPDRAWITEMSTRUCT)lParam;
            FillRect (pdis -> hDC, &pdis -> rcItem, (HBRUSH)CreateSolidBrush(RGB(50, 100, 50)));
            FrameRect(pdis -> hDC, &pdis -> rcItem, (HBRUSH)CreateSolidBrush(RGB(0, 0, 0)));

            SetBkMode(pdis->hDC, TRANSPARENT);
            SetTextColor(pdis->hDC, RGB(0, 255, 100));

            if(pdis -> CtlID == ID_BUTTON_DRAW) {
                DrawText(pdis -> hDC, L"Draw", -1, &pdis -> rcItem, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
            }

            if(pdis -> CtlID == ID_BUTTON_CLEAR) {
                DrawText(pdis -> hDC, L"Clear", -1, &pdis -> rcItem, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
            }

            break;
        }



        case WM_COMMAND: {

            switch(LOWORD(wParam)) {

                case ID_BUTTON_DRAW: {

                    HDC hdc = GetDC(hwnd);

                    Rectangle(hdc, 250, 200, 550, 400);
                    Rectangle(hdc, 350, 250, 450, 400);

                    MoveToEx(hdc, 200, 200, NULL);
                    LineTo(hdc, 400, 50);
                    LineTo(hdc, 600, 200);
                    LineTo(hdc, 200, 200);

                    Ellipse(hdc, 270, 270, 330, 330);
                    Ellipse(hdc, 470, 270, 530, 330);

                    Ellipse(hdc, 350, 120, 450, 180);
                    Ellipse(hdc, 375, 130, 425, 170);

                    Arc(hdc, 350, 400, 450, 450, 350, 400, 450, 850);
                    Arc(hdc, 425, 400, 525, 450, 425, 400, 525, 850);


                    break;

                }

                case ID_BUTTON_CLEAR: {

                    HDC hdc = GetDC(hwnd);
                    RECT rect;
                    GetClientRect(hwnd, &rect);
                    rect.bottom -= 70;
                    FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));

                    break;
                }

            }

            break;
        }

        case WM_GETMINMAXINFO: {
            MINMAXINFO* mmi = (MINMAXINFO*)lParam;
            mmi -> ptMinTrackSize.x = 800;
            mmi -> ptMinTrackSize.y = 600;
            mmi -> ptMaxTrackSize.x = 800;
            mmi -> ptMaxTrackSize.y = 600;
            break;
        }

        case WM_CLOSE: {
            DestroyWindow(hwnd);
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
        L"Sys Programming, Lab: 3",
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

    while(GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

