#include <Windows.h>
#include "window_class.h"
#include <iostream>
#include "DX12App.h"
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    DX12App* pFramework = reinterpret_cast<DX12App*>(
        GetWindowLongPtr(hwnd, GWLP_USERDATA));
    switch (msg)
    {
    case WM_CREATE:
    {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);

        SetWindowLongPtr(hwnd, GWLP_USERDATA,
            reinterpret_cast<LONG_PTR>(pCreate->lpCreateParams));
    }
    case WM_INPUT:
    {
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
            if (raw->data.mouse.usFlags == MOUSE_MOVE_RELATIVE)
            {
                //int x = raw->data.mouse.lLastX;
                //int y = raw->data.mouse.lLastY;
                pFramework->OnMouseMove(wParam, raw->data.mouse.lLastX, raw->data.mouse.lLastY);
            }

            USHORT buttonFlags = raw->data.mouse.usButtonFlags;
            if (buttonFlags & RI_MOUSE_LEFT_BUTTON_DOWN) {
                pFramework->OnMouseDown(hwnd, raw->data.mouse.lLastX, raw->data.mouse.lLastY);
            }
            if (buttonFlags & RI_MOUSE_LEFT_BUTTON_UP) {
                pFramework->OnMouseUp();
            }
        }

        else if (raw->header.dwType == RIM_TYPEKEYBOARD)
        {
            RAWKEYBOARD& keyboard = raw->data.keyboard;

            UINT virtualKey = keyboard.VKey;
            UINT scanCode = keyboard.MakeCode;
            UINT flags = keyboard.Flags;

            bool keyDown = !(flags & RI_KEY_BREAK);

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