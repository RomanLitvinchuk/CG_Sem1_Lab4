#include <Windows.h>
#include "game_timer.h"
#include "window_class.h"

int WindowClass::WRun(GameTimer* gt, DX12App* framework) {
	MSG msg = {};
	gt->Reset();
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
		else {
			gt->Tick();
			framework->CalculateGameStats(*gt, hWnd_);
			framework->Update(*gt);
			framework->Draw(*gt);
			
		}

	}
	return (int)msg.wParam;
}
