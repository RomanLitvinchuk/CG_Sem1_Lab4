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
	ComPtr<ID3D12Device> m_device_;
	ComPtr<ID3D12Fence> m_fence_;
	UINT mRTVDescriptorSize_ = 0;
	UINT mDSVDescriptorSize_ = 0;
	UINT mCbvSrvUavDescriptorSize_ = 0;
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels_;
	DXGI_FORMAT mBackBufferFormat_;
};