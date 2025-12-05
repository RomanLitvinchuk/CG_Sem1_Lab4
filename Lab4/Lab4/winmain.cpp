#include <Windows.h>
#include <d3d12.h>
#include <wrl.h>
#include <dxgi.h>
#include <DirectXHelpers.h>
#include <comdef.h>
#include "window_class.h"
#include "DX12App.h"
#include "game_timer.h"
#include "vertex.h"
#include "d3dUtil.h"
#pragma comment(linker, "/entry:wWinMainCRTStartup")
HWND g_hWnd = 0;

using namespace DirectX;
using namespace DX12;
using namespace Microsoft::WRL;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	DX12App MyFramework;
	WindowClass wnd(hInstance, hPrevInstance, &MyFramework);
	wnd.initWindow(WndProc);
	wnd.CheckRegister();
	wnd.CreateWnd();
	if (!wnd.CheckCreation()) {
		return 0;
	}
	g_hWnd = wnd.getHWND();

	MyFramework.InitializeDevice();
	MyFramework.InitializeCommandObjects();
	MyFramework.CreateSwapChain(g_hWnd);
	MyFramework.CreateRTVAndDSVDescriptorHeaps();
	MyFramework.CreateRTV();
	MyFramework.CreateDSV();
	MyFramework.SetViewport();
	MyFramework.SetScissor();
	MyFramework.SetTopology();
	MyFramework.BuildLayout();
	MyFramework.CreateVertexBuffer();
	MyFramework.CreateIndexBuffer();
	MyFramework.InitUploadBuffer();
	MyFramework.CreateCBVDescriptorHeap();
	MyFramework.CreateConstantBufferView();
	MyFramework.CreateRootSignature();
	MyFramework.CompileShaders();
	MyFramework.CreatePSO();
	GameTimer gt;


	wnd.RegisterRawInputDevice();
	wnd.ShowWnd();

	wnd.WRun(&gt, &MyFramework);

	return 0;
}