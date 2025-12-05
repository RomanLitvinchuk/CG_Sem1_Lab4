#define _WINDOW_CLASS_
#include <Windows.h>
#include "game_timer.h"
#include "DX12App.h"
class WindowClass 
{
private:
	HINSTANCE hInstance_;
	HINSTANCE hPrevInstance_;
	WNDCLASSEX wc_;
	HWND hWnd_ = nullptr;
	RAWINPUTDEVICE rid_[2];
	DX12App* framework_;
public:
	WindowClass(HINSTANCE hInstance, HINSTANCE hPrevInstance, DX12App* pFramework) : hInstance_(hInstance), hPrevInstance_(hPrevInstance), framework_(pFramework) {}
	void initWindow(WNDPROC WndProc);
	void CreateWnd();
	void ShowWnd();
	bool CheckRegister();
	void RegisterRawInputDevice();
	int WRun(GameTimer* gt, DX12App* framework);
	bool CheckCreation();
	HWND getHWND() const { return hWnd_; }
};
