#ifndef UNICODE
#define UNICODE
#define UNICODE_WAS_UNDEFINED
#endif

#include <Windows.h>
#include <wchar.h>
#include <tchar.h>
#include <math.h>

#ifdef UNICODE_WAS_UNDEFINED
#undef UNICODE
#endif

#define PI 3.14
#define ID_CHECK_BOX 228
#define WS_RADIOBUTTON_PARAMS WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON

const wchar_t* colors[3] = {
    L"Red",
    L"Green",
    L"Blue"
};

const wchar_t* figures[4] = {
    L"Star",
    L"Circle",
    L"Rhombus",
    L"Rectangle"
};

const int colors_id[3] = {
    101,
    102,
    103
};

const int figures_id[4] = {
    104,
    105,
    106,
    107
};

int RGB[3];
int x_coord = 0, y_coord = 0;

HWND check_box,
     child_hwnd,
     color_radio_buttons[3],
     figure_radio_buttons[4];

HINSTANCE hInst;

void DrawFigure(HDC hdc, int type) {

    HBRUSH hBrush = CreateSolidBrush(RGB(RGB[0], RGB[1], RGB[2]));
    SelectObject(hdc, hBrush);

    switch(type) {

        case 0: {

            POINT point[5];
            for(int i = 0; i < 5; ++i) {
                point[i].x = x_coord + sin(2 * PI / 5 * (i * 2 % 5)) * 30;
                point[i].y = y_coord - cos(2 * PI / 5 * (i * 2 % 5)) * 30;
            }

            Polygon(hdc, point, 5);

            break;
        }

        case 1: {
            Ellipse(hdc, x_coord - 30, y_coord - 30, x_coord + 30, y_coord + 30);
            break;
        }

        case 2: {

            POINT point[4];

            point[0].x = x_coord - 30;
            point[0].y = y_coord;

            point[1].x = x_coord;
            point[1].y = y_coord - 30;

            point[2].x = x_coord + 30;
            point[2].y = y_coord;

            point[3].x = x_coord;
            point[3].y = y_coord + 30;

            Polygon(hdc, point, 4);

            break;
        }

        case 3: {
            Rectangle(hdc, x_coord - 30, y_coord - 30, x_coord + 30, y_coord + 30);
            break;
        }

    }

}


LRESULT CALLBACK WndProcChild(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam
) {

    switch(msg) {

        case WM_LBUTTONDOWN : {

            x_coord = LOWORD(lParam);
            y_coord = HIWORD(lParam);
            InvalidateRect(child_hwnd, 0, 0);
            break;

        }

        case WM_PAINT: {

            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            if(SendMessage(check_box, BM_GETCHECK, 1, NULL)) {

                for(int i = 0; i < 3; ++i) {
                    if(SendMessage(color_radio_buttons[i], BM_GETCHECK, 1, 0)) {
                        RGB[i] = 255;
                    } else {
                        RGB[i] = 0;
                    }
                }

                for(int i = 0; i < 4; ++i) {
                    if(SendMessage(figure_radio_buttons[i], BM_GETCHECK, 1, NULL)) {
                        DrawFigure(hdc, i);
                    }
                }
            }

            EndPaint(hwnd, &ps);
        }

        default: {
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }
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

            WNDCLASS wc;
            memset(&wc, 0, sizeof(WNDCLASS));

            wc.style         = 0;
            wc.hInstance     = hInst;
            wc.lpfnWndProc   = WndProcChild;
            wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
            wc.lpszClassName = L"C Windows";
            wc.hCursor       = LoadCursor(NULL, IDC_ARROW);

            RegisterClass(&wc);

            child_hwnd = CreateWindow(
                L"C Windows", L"Lab5. Child Window.",
                WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX,
                220, 180, 600, 480, NULL, NULL, hInst, NULL
            );

            ShowWindow(child_hwnd, SW_NORMAL);
            UpdateWindow(child_hwnd);

            if (child_hwnd == NULL) {
                MessageBox(hwnd, L"Can't create child window.", L"Error!", MB_ICONERROR | MB_OK);
            }

            for(size_t i = 0; i < 3; ++i) {
                color_radio_buttons[i] = CreateWindow(
                    L"BUTTON", colors[i], WS_RADIOBUTTON_PARAMS,
                    10, 10 + 15 * (i + 1), 100, 20, hwnd, (HMENU)colors_id[i], NULL, NULL
                );
            }

            for(size_t i = 0; i < 4; ++i) {
                figure_radio_buttons[i] = CreateWindow(
                    L"BUTTON", figures[i], WS_RADIOBUTTON_PARAMS,
                    10, 65 + 15 * (i + 1), 100, 20, hwnd, (HMENU)figures_id[i], NULL, NULL
                );
            }

            check_box = CreateWindow(
                L"BUTTON", L"Check", WS_VISIBLE | WS_CHILD | BS_CHECKBOX,
                10, 160, 100, 20, hwnd, (HMENU)ID_CHECK_BOX, NULL, NULL
            );

            break;
        }

        case WM_COMMAND: {

            int target_id = LOWORD(wParam);

            if(target_id == ID_CHECK_BOX) {

                bool status = SendDlgItemMessage(hwnd, ID_CHECK_BOX, BM_GETCHECK, 0, 0);
                SendMessage(check_box, BM_SETCHECK, status ^ true, NULL);

            } else if(target_id >= 101 && target_id <= 103) {

                for(int i = 0; i < 3; ++i) {
                   SendMessage((HWND)color_radio_buttons[i], BM_SETCHECK, 0, NULL);
                }
                SendMessage(color_radio_buttons[target_id - 101], BM_SETCHECK, 1, NULL);

            } else if(target_id >= 104 && target_id <= 107) {

                for(int i = 0; i < 4; ++i) {
                    SendMessage((HWND)figure_radio_buttons[i], BM_SETCHECK, 0, NULL);
                }
                SendMessage((HWND)figure_radio_buttons[target_id - 104], BM_SETCHECK, 1, NULL);
            }

            break;
        }

        case WM_CLOSE: {
            DestroyWindow(hwnd);
            break;
        }

        case WM_DESTROY: {
            PostQuitMessage(0);
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
        L"Sys Programming, Lab: 5",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 130, 250,
        NULL, NULL, hInstance, NULL
    );

    if(hwnd == NULL) {
        MessageBox(NULL, L"Window creation is failed!", L"Error!", MB_ICONWARNING | MB_OK);
        return 0;
    }

    hInst = hInstance;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while(GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

