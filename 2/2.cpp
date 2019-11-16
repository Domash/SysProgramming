#ifndef UNICODE
#define UNICODE
#define UNICODE_WAS_UNDEFINED
#endif

#include <Windows.h>
#include <wchar.h>
#include <tchar.h>

#ifdef UNICODE_WAS_UNDEFINED
#undef UNICODE
#endif

#define ID_TEXT_BOX    101
#define ID_LIST_BOX_1  102
#define ID_LIST_BOX_2  103
#define ID_ADD_BUTTON  104
#define ID_DEL_BUTTON  105
#define ID_CLS_BUTTON  106
#define ID_TO_R_BUTTON 107

#define WS_PARAMS WS_BORDER | WS_CHILD | WS_VISIBLE

#define GET_VARIABLE_NAME(Variable) (#Variable)

HWND text_box,
     list_box_1,
     list_box_2,
     add_button,
     del_button,
     to_r_button,
     clear_button;

const wchar_t* ConvertToWChar(const char* str) {

    const size_t str_size = strlen(str) + 1;
    wchar_t* wchar_str = new wchar_t[str_size];
    mbstowcs (wchar_str, str, str_size);

    return wchar_str;
}

void PrintErrorMessage(HWND hwnd, const char* name) {
    
    wchar_t* message = (wchar_t*) malloc(100 * sizeof(wchar_t));
    wcscpy(message, L"Failed to create: ");
    wcscat(message, ConvertToWChar(name));

    MessageBox(hwnd, message, L"Error!", MB_OK | MB_ICONERROR);
}

template <typename WinApiType>
void CheckForErrors(HWND hwnd, WinApiType object, const char* object_name) {
    if(object == NULL) PrintErrorMessage(hwnd, object_name);
}

LRESULT CALLBACK WndProc(
    HWND   hwnd,
    UINT   msg,
    WPARAM wParam,
    LPARAM lParam
) {

    switch(msg) {

        case WM_CREATE: {

            text_box = CreateWindow(L"EDIT", L"", WS_PARAMS, 200, 40, 400, 30, hwnd, (HMENU) ID_TEXT_BOX, NULL, NULL);
            CheckForErrors(hwnd, text_box, GET_VARIABLE_NAME(text_box));

            list_box_1 = CreateWindow(L"LISTBOX", L"", WS_PARAMS, 80, 100, 300, 300, hwnd, (HMENU) ID_LIST_BOX_1, NULL, NULL);
            CheckForErrors(hwnd, list_box_1, GET_VARIABLE_NAME(list_box_1));

            list_box_2 = CreateWindow(L"LISTBOX", L"", WS_PARAMS, 420, 100, 300, 300, hwnd, (HMENU) ID_LIST_BOX_2, NULL, NULL);
            CheckForErrors(hwnd, list_box_2, GET_VARIABLE_NAME(list_box_2));

            add_button = CreateWindow(L"Button", L"Add", WS_PARAMS, 100, 420, 100, 30, hwnd, (HMENU) ID_ADD_BUTTON, NULL, NULL);
            CheckForErrors(hwnd, add_button, GET_VARIABLE_NAME(add_button));

            clear_button = CreateWindow(L"Button", L"Clear", WS_PARAMS, 250, 420, 100, 30, hwnd, (HMENU) ID_CLS_BUTTON, NULL, NULL);
            CheckForErrors(hwnd, clear_button, GET_VARIABLE_NAME(clear_button));

            del_button = CreateWindow(L"Button", L"Delete", WS_PARAMS, 450, 420, 100, 30, hwnd, (HMENU) ID_DEL_BUTTON, NULL, NULL);
            CheckForErrors(hwnd, del_button, GET_VARIABLE_NAME(del_button));

            to_r_button = CreateWindow(L"Button", L"To Right", WS_PARAMS, 600, 420, 100, 30, hwnd, (HMENU) ID_TO_R_BUTTON, NULL, NULL);
            CheckForErrors(hwnd, to_r_button, GET_VARIABLE_NAME(to_r_button));

            break;
        }

        case WM_COMMAND: {

            switch(LOWORD(wParam)) {

                case ID_ADD_BUTTON: {

                    size_t text_size = GetWindowTextLength(GetDlgItem(hwnd, ID_TEXT_BOX));

                    if(text_size) {

                        wchar_t* buffer = (wchar_t*)GlobalAlloc(GPTR, sizeof(wchar_t) * (text_size + 1));

                        GetDlgItemText(hwnd, ID_TEXT_BOX, buffer, text_size + 1);
                        if(SendDlgItemMessage(hwnd, ID_LIST_BOX_1, LB_FINDSTRINGEXACT, NULL, (LPARAM)buffer) == LB_ERR) {
                            SendDlgItemMessage(hwnd, ID_LIST_BOX_1, LB_ADDSTRING, NULL, LPARAM(buffer));
                        }

                        SetDlgItemText(hwnd, ID_TEXT_BOX, L"");
                        GlobalFree(buffer);

                    }

                    break;
                }

                case ID_DEL_BUTTON: {

                    int cursor = SendDlgItemMessage(hwnd, ID_LIST_BOX_1, LB_GETCURSEL, NULL, NULL);

                    if(cursor != LB_ERR) {
                        SendDlgItemMessage(hwnd, ID_LIST_BOX_1, LB_DELETESTRING, cursor, NULL);
                    }

                    cursor = SendDlgItemMessage(hwnd, ID_LIST_BOX_2, LB_GETCURSEL, NULL, NULL);

                    if(cursor != LB_ERR) {
                        SendDlgItemMessage(hwnd, ID_LIST_BOX_2, LB_DELETESTRING, cursor, NULL);
                    }

                    break;
                }

                case ID_CLS_BUTTON: {
                    SendDlgItemMessage(hwnd, ID_LIST_BOX_1, LB_RESETCONTENT, NULL, NULL);
                    SendDlgItemMessage(hwnd, ID_LIST_BOX_2, LB_RESETCONTENT, NULL, NULL);
                    break;
                }

                case ID_TO_R_BUTTON: {

                    int cursor = SendDlgItemMessage(hwnd, ID_LIST_BOX_1, LB_GETCURSEL, NULL, NULL);

                    if(cursor == LB_ERR) {
                        MessageBox(hwnd, L"No selected item, try again", L"Error!", MB_OK | MB_ICONERROR);
                    } else {

                        size_t text_size = SendDlgItemMessage(hwnd, ID_LIST_BOX_1, LB_GETTEXTLEN, cursor, NULL);
                        wchar_t* buffer = (wchar_t*)GlobalAlloc(GPTR, sizeof(wchar_t) * (text_size + 1));

                        SendDlgItemMessage(hwnd, ID_LIST_BOX_1, LB_GETTEXT, cursor, (LPARAM)buffer);
                        if(SendDlgItemMessage(hwnd, ID_LIST_BOX_2, LB_FINDSTRINGEXACT, NULL, (LPARAM)buffer) == LB_ERR) {
                            SendDlgItemMessage(hwnd, ID_LIST_BOX_2, LB_ADDSTRING, NULL, LPARAM(buffer));
                        }

                        GlobalFree(buffer);

                    }

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
        L"Sys Programming, Lab: 2",
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
