#include "DX12App.h"
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


void DX12App::Initialize() {
	EnableDebug();
	ComPtr<IDXGIFactory4> mdxgiFactory;
	ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&mdxgiFactory)));

	ThrowIfFailed(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&m_device_)));
	std::cout << "DEVICE CREATED: " << std::endl;

	ThrowIfFailed(m_device_->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence_)));
	std::cout << "FENCE CREATED" << std::endl;

	mRTVDescriptorSize_ = m_device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	mDSVDescriptorSize_ = m_device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	mCbvSrvUavDescriptorSize_ = m_device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	std::cout << "RTV size: " << std::to_string(mRTVDescriptorSize_) << "\n"
		<< "DSV size: " << std::to_string(mDSVDescriptorSize_) << "\n"
		<< "CbvSrvUav size:" << std::to_string(mCbvSrvUavDescriptorSize_) << std::endl;

	mBackBufferFormat_ = DXGI_FORMAT_R8G8B8A8_UNORM;
	msQualityLevels_.Format = mBackBufferFormat_;
	msQualityLevels_.SampleCount = 4;
	msQualityLevels_.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	msQualityLevels_.NumQualityLevels = 0;

	ThrowIfFailed(m_device_->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &msQualityLevels_, sizeof(msQualityLevels_)));
	if (msQualityLevels_.NumQualityLevels > 0) { std::cout << "MSAA 4x is supported" << std::endl;} 
	else { std::cout << "WARNING! MSAA 4x is NOT supported" << std::endl; }
}