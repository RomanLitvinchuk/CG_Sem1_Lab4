#include "DX12App.h"
#include "d3dx12.h"
#include <iostream>

void DX12App::EnableDebug() {
#if defined(DEBUG) || defined(_DEBUG)
	{
		ComPtr<ID3D12Debug> debugController;
		ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
		debugController->EnableDebugLayer();
	}
#endif
}


void DX12App::InitializeDevice() {
	EnableDebug();
	ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&m_dxgi_factory_)));

	ThrowIfFailed(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&m_device_)));
	std::cout << "DEVICE CREATED: " << std::endl;

	ThrowIfFailed(m_device_->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence_)));
	std::cout << "FENCE CREATED" << std::endl;

	m_RTV_descriptor_size_ = m_device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	m_DSV_descriptor_size_ = m_device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	m_CbvSrvUav_descriptor_size_ = m_device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	std::cout << "RTV size: " << std::to_string(m_RTV_descriptor_size_) << "\n"
		<< "DSV size: " << std::to_string(m_DSV_descriptor_size_) << "\n"
		<< "CbvSrvUav size:" << std::to_string(m_CbvSrvUav_descriptor_size_) << std::endl;

	msQualityLevels_.Format = m_back_buffer_format_;
	msQualityLevels_.SampleCount = 4;
	msQualityLevels_.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	msQualityLevels_.NumQualityLevels = 0;

	ThrowIfFailed(m_device_->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &msQualityLevels_, sizeof(msQualityLevels_)));
	if (msQualityLevels_.NumQualityLevels > 0) { std::cout << "MSAA 4x is supported" << std::endl;} 
	else { std::cout << "WARNING! MSAA 4x is NOT supported" << std::endl; }
}

void DX12App::InitializeCommandObjects() {
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	ThrowIfFailed(m_device_->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_command_queue_)));
	std::cout << "Command queue is created" << std::endl;
	ThrowIfFailed(m_device_->CreateCommandAllocator(queueDesc.Type, IID_PPV_ARGS(&m_direct_cmd_list_alloc_)));
	std::cout << "Command allocatoris is created" << std::endl;
	ThrowIfFailed(m_device_->CreateCommandList(0, queueDesc.Type, m_direct_cmd_list_alloc_.Get(), nullptr, IID_PPV_ARGS(&m_command_list_)));
	std::cout << "Command list is created" << std::endl;
}

void DX12App::CreateSwapChain(HWND hWnd) {
	DXGI_SWAP_CHAIN_DESC swDesc = {};
	swDesc.BufferDesc.Width = m_client_width_;
	swDesc.BufferDesc.Height = m_client_height_;
	swDesc.BufferDesc.RefreshRate.Numerator = 60;
	swDesc.BufferDesc.RefreshRate.Denominator = 1;
	swDesc.BufferDesc.Format = m_back_buffer_format_;
	swDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	//swDesc.SampleDesc.Count = (msQualityLevels_.NumQualityLevels > 0) ? 4 : 1;
	//swDesc.SampleDesc.Quality = (msQualityLevels_.NumQualityLevels > 0) ? msQualityLevels_.NumQualityLevels - 1 : 0;
	swDesc.SampleDesc.Count = 1;
	swDesc.SampleDesc.Quality = 0;
	swDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swDesc.BufferCount = 2;
	swDesc.OutputWindow = hWnd;
	swDesc.Windowed = true;
	swDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	ThrowIfFailed(m_dxgi_factory_->CreateSwapChain(m_command_queue_.Get(), &swDesc, &m_swap_chain_));
	std::cout << "Swap chain is created" << std::endl;
	
}

void DX12App::CreateRTVAndDSVDescriptorHeaps() {
	D3D12_DESCRIPTOR_HEAP_DESC RTVHeapDesc;
	RTVHeapDesc.NumDescriptors = 2;
	RTVHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	RTVHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	RTVHeapDesc.NodeMask = 0;
	ThrowIfFailed(m_device_->CreateDescriptorHeap(&RTVHeapDesc, IID_PPV_ARGS(&m_RTV_heap_)));
	std::cout << "RTV heap is created" << std::endl;

	D3D12_DESCRIPTOR_HEAP_DESC DSVHeapDesc;
	DSVHeapDesc.NumDescriptors = 1;
	DSVHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	DSVHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	DSVHeapDesc.NodeMask = 0;
	ThrowIfFailed(m_device_->CreateDescriptorHeap(&DSVHeapDesc, IID_PPV_ARGS(&m_DSV_heap_)));
	std::cout << "DSV heap is created" << std::endl;
}

D3D12_CPU_DESCRIPTOR_HANDLE DX12App::GetBackBuffer() const {
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_RTV_heap_->GetCPUDescriptorHandleForHeapStart(), m_current_back_buffer_, m_RTV_descriptor_size_);
}

D3D12_CPU_DESCRIPTOR_HANDLE DX12App::GetDSV() const {
	return m_DSV_heap_->GetCPUDescriptorHandleForHeapStart();
}