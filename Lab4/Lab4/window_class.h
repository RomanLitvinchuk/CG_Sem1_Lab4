#define _WINDOW_CLASS_
#include <Windows.h>

class WindowClass 
{
private:
	HINSTANCE hInstance_;
	HINSTANCE hPrevInstance_;
	WNDCLASSEX wc_;
	HWND hWnd_ = nullptr;
public:
	WindowClass(HINSTANCE hInstance, HINSTANCE hPrevInstance) : hInstance_(hInstance), hPrevInstance_(hPrevInstance) {}
	void initWindow(WNDPROC WndProc);
	void CreateWnd();
	void ShowWnd();
	void UpdateWnd();
	bool CheckRegister();
	bool CheckCreation();
};
