#include <Windows.h>
#include <windowsx.h>
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
        return 0;
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
            // Относительное движение
            short dx = raw->data.mouse.lLastX;
            short dy = raw->data.mouse.lLastY;

            USHORT buttons = raw->data.mouse.usButtonFlags;

            if (buttons & RI_MOUSE_LEFT_BUTTON_DOWN)
                pFramework->OnMouseDown(hwnd);

            if (buttons & RI_MOUSE_LEFT_BUTTON_UP)
                pFramework->OnMouseUp();

            bool leftDown = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
            pFramework->OnMouseMove(leftDown ? MK_LBUTTON : 0, dx, dy);
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

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_ERASEBKGND:
        return 1;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}