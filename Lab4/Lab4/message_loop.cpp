#include <Windows.h>
#include "game_timer.h"
#include "window_class.h"

int WindowClass::WRun(GameTimer* gt, DX12App* framework) {
    MSG msg = {};
    gt->Reset();

    while (true) {
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                break;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            gt->Tick();

            if (framework) {
                framework->CalculateGameStats(*gt, hWnd_);
                framework->Update(*gt);
                framework->Draw(*gt);
            }
        }
    }

    return static_cast<int>(msg.wParam);
}
