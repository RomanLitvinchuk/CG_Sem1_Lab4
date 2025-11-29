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
	void InitializeDevice();
	void InitializeCommandObjects();
	void CreateSwapChain(HWND hWnd);
private:
	void EnableDebug();

	DXGI_FORMAT m_back_buffer_format_ = DXGI_FORMAT_R8G8B8A8_UNORM;
	UINT m_client_width_ = 800;
	UINT m_client_height_ = 600;

	ComPtr<IDXGIFactory4> m_dxgi_factory_;
	ComPtr<ID3D12Device> m_device_;
	ComPtr<ID3D12Fence> m_fence_;
	UINT mRTVDescriptorSize_ = 0;
	UINT mDSVDescriptorSize_ = 0;
	UINT mCbvSrvUavDescriptorSize_ = 0;
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels_;
	

	ComPtr<ID3D12CommandQueue> m_command_queue_;
	ComPtr<ID3D12CommandAllocator> m_direct_cmd_list_alloc_;
	ComPtr<ID3D12CommandList> m_command_list_;
	
	ComPtr<IDXGISwapChain> m_swap_chain_;
};