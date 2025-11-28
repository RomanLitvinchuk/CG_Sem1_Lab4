#include <Windows.h>
#include "window_class.h"
#include <iostream>
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_INPUT:
    {
        std::cout << "WM_INPUT received!" << std::endl;
        UINT dwSize = 0;

        GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize,
            sizeof(RAWINPUTHEADER));

        BYTE* lpb = new BYTE[dwSize];
        if (lpb == NULL) return 0;

        if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize,
            sizeof(RAWINPUTHEADER)) != dwSize) {
            delete[] lpb;
            return 0;
        }

        RAWINPUT* raw = (RAWINPUT*)lpb;

        if (raw->header.dwType == RIM_TYPEMOUSE)
        {
            std::cout << "Mouse input: X=" << raw->data.mouse.lLastX
                << " Y=" << raw->data.mouse.lLastY << std::endl;
            if (raw->data.mouse.usFlags == MOUSE_MOVE_RELATIVE)
            {
                int x = raw->data.mouse.lLastX;
                int y = raw->data.mouse.lLastY;
            }

            USHORT buttonFlags = raw->data.mouse.usButtonFlags;
            if (buttonFlags & RI_MOUSE_LEFT_BUTTON_DOWN) {
            }
            if (buttonFlags & RI_MOUSE_LEFT_BUTTON_UP) {
            }
        }

        else if (raw->header.dwType == RIM_TYPEKEYBOARD)
        {
            std::cout << "Keyboard input: VKey=" << raw->data.keyboard.VKey
                << " ScanCode=" << raw->data.keyboard.MakeCode << std::endl;
            RAWKEYBOARD& keyboard = raw->data.keyboard;

            UINT virtualKey = keyboard.VKey;
            UINT scanCode = keyboard.MakeCode;
            UINT flags = keyboard.Flags;

            bool keyDown = !(flags & RI_KEY_BREAK);

            std::cout << "Key " << (keyDown ? "DOWN" : "UP")
                << ": " << virtualKey << std::endl;
        }

        delete[] lpb;
        return 0;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}