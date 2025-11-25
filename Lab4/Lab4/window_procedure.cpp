#include <Windows.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam) 
{
	switch (uMessage) 
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		DefWindowProc(hWnd, uMessage, wParam, lParam);
	}
}