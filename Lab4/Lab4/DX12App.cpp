#include "DX12App.h"
#include "d3dx12.h"
#include <iostream>
#include <string>
#include <DirectXColors.h>

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
	ThrowIfFailed(m_command_list_->Close());
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

ID3D12Resource* DX12App::CurrentBackBuffer() const {
	return m_swap_chain_buffer_[m_current_back_buffer_].Get();
}


void DX12App::CreateRTV() {
	CD3DX12_CPU_DESCRIPTOR_HANDLE RTV_heap_handle_(m_RTV_heap_->GetCPUDescriptorHandleForHeapStart());
	for (UINT i = 0; i < 2; i++) {
		ThrowIfFailed(m_swap_chain_->GetBuffer(i, IID_PPV_ARGS(&m_swap_chain_buffer_[i])));
		m_device_->CreateRenderTargetView(m_swap_chain_buffer_[i].Get(), nullptr, RTV_heap_handle_);
		RTV_heap_handle_.Offset(1, m_RTV_descriptor_size_);
	}
	std::cout << "RTV is created" << std::endl;
}

void DX12App::CreateDSV() {
	D3D12_RESOURCE_DESC dsDesc;
	dsDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	dsDesc.Alignment = 0;
	dsDesc.Width = m_client_width_;
	dsDesc.Height = m_client_height_;
	dsDesc.DepthOrArraySize = 1;
	dsDesc.MipLevels = 1;
	dsDesc.Format = m_depth_stencil_format_;
	dsDesc.SampleDesc.Count = 1;
	dsDesc.SampleDesc.Quality = 0;
	dsDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	dsDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_CLEAR_VALUE clrValue;
	clrValue.Format = m_depth_stencil_format_;
	clrValue.DepthStencil.Depth = 1.0f;
	clrValue.DepthStencil.Stencil = 0;
	CD3DX12_HEAP_PROPERTIES heapProperties(D3D12_HEAP_TYPE_DEFAULT);
	ThrowIfFailed(m_device_->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &dsDesc, D3D12_RESOURCE_STATE_COMMON, &clrValue, IID_PPV_ARGS(&m_DSV_buffer)));
	std::cout << "DSV is created" << std::endl;
	m_device_->CreateDepthStencilView(m_DSV_buffer.Get(), nullptr, GetDSV());
	CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_DSV_buffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);
	m_command_list_->ResourceBarrier(1, &barrier);
}

void DX12App::SetViewport() {
	vp_.TopLeftX = 0.0f;
	vp_.TopLeftY = 0.0f;
	vp_.Width = static_cast<float>(m_client_width_);
	vp_.Height = static_cast<float>(m_client_height_);
	vp_.MinDepth = 0.0f;
	vp_.MaxDepth = 1.0f;
	m_command_list_->RSSetViewports(1, &vp_);
	std::cout << "Viewport is set" << std::endl;
}

void DX12App::SetScissor() {
	m_scissor_rect_ = { 0, 0, 400, 300 };
	m_command_list_->RSSetScissorRects(1, &m_scissor_rect_);
	std::cout << "Scissor is set" << std::endl;
}

void DX12App::CalculateGameStats(GameTimer& gt, HWND hWnd) {
	static int frameCnt= 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	if ((gt.TotalTime() - timeElapsed) >= 1.0f) {
		float fps = (float)frameCnt;
		float mspf = 1000.0f / fps;
		std::wstring WindowString = L"WINDOW  fps: " + std::to_wstring(fps) + L"mspf: " + std::to_wstring(mspf);
		SetWindowText(hWnd, WindowString.c_str());

		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}

void DX12App::WaitForGPU()
{
	if (m_fence_->GetCompletedValue() < m_current_fence_)
	{
		HANDLE eventHandle = CreateEventEx(nullptr, nullptr, false, EVENT_ALL_ACCESS);
		m_fence_->SetEventOnCompletion(m_current_fence_, eventHandle);
		WaitForSingleObject(eventHandle, INFINITE);
		CloseHandle(eventHandle);
	}
}


void DX12App::Draw(const GameTimer& gt) {
	WaitForGPU();
	ThrowIfFailed(m_command_list_->Reset(m_direct_cmd_list_alloc_.Get(), nullptr));
	CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	m_command_list_->ResourceBarrier(1, &barrier);
	m_command_list_->RSSetViewports(1, &vp_);
	m_command_list_->RSSetScissorRects(1, &m_scissor_rect_);
	m_command_list_->ClearRenderTargetView(GetBackBuffer(), Colors::LightSteelBlue, 0, nullptr);
	m_command_list_->ClearDepthStencilView(GetDSV(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
	D3D12_CPU_DESCRIPTOR_HANDLE backBuffer = GetBackBuffer();
	D3D12_CPU_DESCRIPTOR_HANDLE dsv = GetDSV();
	m_command_list_->OMSetRenderTargets(1, &backBuffer, true, &dsv);
	CD3DX12_RESOURCE_BARRIER barrier2 = CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	m_command_list_->ResourceBarrier(1, &barrier2);
	ThrowIfFailed(m_command_list_->Close());
	ID3D12CommandList* cmdLists[] = { m_command_list_.Get() };
	m_command_queue_->ExecuteCommandLists(_countof(cmdLists), cmdLists);
	ThrowIfFailed(m_swap_chain_->Present(0, 0));
	m_current_back_buffer_ = (m_current_back_buffer_ + 1) % 2;
	m_current_fence_++;
	m_command_queue_->Signal(m_fence_.Get(), m_current_fence_);
}