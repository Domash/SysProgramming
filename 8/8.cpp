#ifndef UNICODE
#define UNICODE
#define UNICODE_WAS_UNDEFINED
#endif

#include <Windows.h>
#include <TlHelp32.h>

#ifdef UNICODE_WAS_UNDEFINED
#undef UNICODE
#endif

#define P_LIST_BOX 201
#define M_LIST_BOX 202

HWND processes_list_box, modules_list_box;

void GetProcessesList() {

    PROCESSENTRY32 pe32;
    wchar_t buffer[1024];
    HANDLE hProcessSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if(hProcessSnapshot == INVALID_HANDLE_VALUE) {
        return;
    }

    pe32.dwSize = sizeof(PROCESSENTRY32);
    Process32First(hProcessSnapshot, &pe32);

    do {
        wsprintf(buffer, L"%08X %s", pe32.th32ProcessID, pe32.szExeFile);
        SendMessage(processes_list_box, LB_ADDSTRING, NULL, (LPARAM)buffer);
    } while(Process32Next(hProcessSnapshot, &pe32));

    CloseHandle(hProcessSnapshot);
}

void GetModulesList() {

    MODULEENTRY32 me32;
    wchar_t buffer[1024];
    HANDLE hModuleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, NULL);

    if(hModuleSnapshot == INVALID_HANDLE_VALUE) {
        return;
    }

    me32.dwSize = sizeof(MODULEENTRY32);
    Module32First(hModuleSnapshot, &me32);

    do {
        wsprintf(buffer, L"%08X, %08X, %s", me32.modBaseAddr, me32.modBaseSize, me32.szModule);
        SendMessage(modules_list_box, LB_ADDSTRING, NULL, (LPARAM)buffer);
    } while(Module32Next(hModuleSnapshot, &me32));

    CloseHandle(hModuleSnapshot);

}

LRESULT CALLBACK WndProc(
    HWND   hwnd,
    UINT   msg,
    WPARAM wParam,
    LPARAM lParam
) {

    switch(msg) {

        case WM_CREATE: {

            processes_list_box = CreateWindow(
                L"LISTBOX", L"p_list", WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL,
                5, 5, 380, 550, hwnd, (HMENU)P_LIST_BOX, NULL, NULL
            );

            modules_list_box = CreateWindow(
                L"LISTBOX", L"m_list", WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL,
                395, 5, 380, 550, hwnd, (HMENU)M_LIST_BOX, NULL, NULL
            );

            GetProcessesList();
            GetModulesList();

            break;
        }

        case WM_COMMAND: {

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
        L"Sys Programming, Lab: 8",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX,
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

