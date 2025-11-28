#include <Windows.h>
#include "window_class.h"
#pragma comment(linker, "/entry:wWinMainCRTStartup")
//HWND g_hWnd = 0;


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);
int Run();

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{

	WindowClass wnd(hInstance, hPrevInstance);
	wnd.initWindow(WndProc);
	wnd.CheckRegister();
	wnd.CreateWnd();
	
	if (!wnd.CheckCreation()) {
		return 0;
	}
	wnd.RegisterRawInputDevice();
	wnd.ShowWnd();
	wnd.UpdateWnd();

	Run();

	return 0;
}