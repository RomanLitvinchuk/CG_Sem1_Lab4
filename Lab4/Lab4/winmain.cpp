#include <Windows.h>
#include <d3d12.h>
#include <wrl.h>
#include <dxgi.h>
#include <DirectXHelpers.h>
#include <comdef.h>
#include "window_class.h"
#include "DX12App.h"
#pragma comment(linker, "/entry:wWinMainCRTStartup")
HWND g_hWnd = 0;

using namespace DirectX;
using namespace DX12;
using namespace Microsoft::WRL;

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
	g_hWnd = wnd.getHWND();

	DX12App MyFramework;
	MyFramework.InitializeDevice();
	MyFramework.InitializeCommandObjects();
	MyFramework.CreateSwapChain(g_hWnd);

	wnd.RegisterRawInputDevice();
	wnd.ShowWnd();
	wnd.UpdateWnd();

	Run();

	return 0;
}