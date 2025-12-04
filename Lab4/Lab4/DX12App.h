#ifndef DX12APP_
#define DX12APP_
#include <Windows.h>
#include <d3d12.h>
#include <DirectXHelpers.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <comdef.h>
#include <DescriptorHeap.h>
#include <d3dx12.h>
#include "throw_if_failed.h"
#include "game_timer.h"

using namespace Microsoft::WRL;
using namespace DirectX;

class DX12App
{
public:
	void InitializeDevice();
	void InitializeCommandObjects();
	void CreateSwapChain(HWND hWnd);
	void CreateRTVAndDSVDescriptorHeaps();
	D3D12_CPU_DESCRIPTOR_HANDLE GetBackBuffer() const;
	ID3D12Resource* CurrentBackBuffer() const;
	D3D12_CPU_DESCRIPTOR_HANDLE GetDSV() const;
	void CreateRTV();
	void CreateDSV();
	void SetViewport();
	void SetScissor();
	void CalculateGameStats(GameTimer& gt, HWND hWnd);
	void Update(const GameTimer& gt) {};
	void Draw(const GameTimer& gt);
	void WaitForGPU();
private:
	void EnableDebug();

	DXGI_FORMAT m_back_buffer_format_ = DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGI_FORMAT m_depth_stencil_format_ = DXGI_FORMAT_D24_UNORM_S8_UINT;
	UINT m_client_width_ = 800;
	UINT m_client_height_ = 600;

	ComPtr<IDXGIFactory4> m_dxgi_factory_;
	ComPtr<ID3D12Device> m_device_;
	ComPtr<ID3D12Fence> m_fence_;
	UINT64 m_current_fence_ = 0;
	UINT m_RTV_descriptor_size_ = 0;
	UINT m_DSV_descriptor_size_ = 0;
	UINT m_CbvSrvUav_descriptor_size_ = 0;
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels_;
	

	ComPtr<ID3D12CommandQueue> m_command_queue_;
	ComPtr<ID3D12CommandAllocator> m_direct_cmd_list_alloc_;
	ComPtr<ID3D12GraphicsCommandList> m_command_list_;
	
	ComPtr<IDXGISwapChain> m_swap_chain_;

	ComPtr<ID3D12DescriptorHeap> m_RTV_heap_;
	ComPtr<ID3D12DescriptorHeap> m_DSV_heap_;
	int m_current_back_buffer_ = 0;

	ComPtr<ID3D12Resource> m_swap_chain_buffer_[2];
	ComPtr<ID3D12Resource> m_DSV_buffer;

	D3D12_VIEWPORT vp_;
	D3D12_RECT m_scissor_rect_;

};

#endif //DX12APP_