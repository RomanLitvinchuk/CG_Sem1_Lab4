#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <DirectXHelpers.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <comdef.h>
#include "throw_if_failed.h"

using namespace Microsoft::WRL;
using namespace DirectX;

class DX12App
{
public:
	void Initialize();
private:
	void EnableDebug();
};