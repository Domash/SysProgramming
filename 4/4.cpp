#define WIN32_MEAN_AND_LEAN
#include <windows.h>

// a helper function to create two buttons
void CreateButtons (HWND parent)
{
  // create two button
  // Here, I've used the standard button class. If you want to have red buttons,
  // either create a new class for the buttons and implement all the functionality
  // yourself, or sub class the standard button and override the drawing functions.
  CreateWindowEx (0,
                  TEXT ("BUTTON"),
                  TEXT ("Button1"),
                  WS_CHILD | WS_VISIBLE,
                  10,
                  10,
                  100,
                  50,
                  parent,
                  0,
                  reinterpret_cast <HINSTANCE> (GetWindowLongPtr (parent, GWL_HINSTANCE)),
                  0);

  CreateWindowEx (0,
                  TEXT ("BUTTON"),
                  TEXT ("Button2"),
                  WS_CHILD | WS_VISIBLE,
                  140,
                  10,
                  100,
                  50,
                  parent,
                  0,
                  reinterpret_cast <HINSTANCE> (GetWindowLongPtr (parent, GWL_HINSTANCE)),
                  0);

}

LRESULT CALLBACK MainWindowProc (HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
  LRESULT
    result;

  bool
    use_default_proc = true;

  switch (message)
  {
  case WM_CREATE:
    // when the window is created, create the two buttons!
    CreateButtons (window);
    break;

  case WM_DESTROY:
    // when the window is finished with, call PostQuitMessage to exit the message loop
    PostQuitMessage (0);
    use_default_proc = false;
    result = 0;
    break;

  case WM_ERASEBKGND:
    // here we draw the black line between the two buttons using a solid colour filled rectangle.
    // this can just as easily be done in the WM_PAINT handler
    {
      // DefWindowProc will clear the window using the WNDCLASSEX.hbrBackground member
      result = DefWindowProc (window, message, w_param, l_param);
      use_default_proc = false;

      // get the DC to draw with
      HDC
        dc = reinterpret_cast <HDC> (w_param);

      // draw the black bar
      RECT
        rect = {120, 0, 130, 70};

      FillRect (dc, &rect, reinterpret_cast <HBRUSH> (GetStockObject (BLACK_BRUSH)));
    }
    break;
  }

  if (use_default_proc)
  {
    result = DefWindowProc (window, message, w_param, l_param);
  }

  return result;
}

int __stdcall WinMain (HINSTANCE instance, HINSTANCE previous_instance, LPSTR command_line, int show_command)
{
  // create a window class for the main window
  WNDCLASSEX
    window_class = 
    {
      sizeof window_class,
      CS_HREDRAW | CS_VREDRAW,
      MainWindowProc,
      0,
      0,
      instance,
      0,
      LoadCursor (0, IDC_ARROW),
      reinterpret_cast <HBRUSH> (COLOR_WINDOW + 1),
      0,
      TEXT ("MainWindowClass"),
      0
    };

  // register the window class
  RegisterClassEx (&window_class);

  // create the main window
  HWND
    window = CreateWindowEx (WS_EX_APPWINDOW,
                             TEXT ("MainWindowClass"),
                             TEXT ("Example Window"),
                             WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                             CW_USEDEFAULT,
                             CW_USEDEFAULT,
                             CW_USEDEFAULT,
                             CW_USEDEFAULT,
                             0,
                             0,
                             instance,
                             0);

  bool
    quit = false;

  while (!quit)
  {
    MSG
      message;

    int
      error_code;

    switch (GetMessage (&message, 0, 0, 0))
    {
    case 0: // WM_QUIT processed
      quit = true;
      break;

    case -1: // an error
      error_code = GetLastError ();
      quit = true;
      break;

    default: // pass the message on to the window...
      TranslateMessage (&message);
      DispatchMessage (&message);
      break;
    }
  }

  return 0;
}