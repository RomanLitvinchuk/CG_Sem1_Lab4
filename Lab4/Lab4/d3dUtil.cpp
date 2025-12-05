#include "d3dUtil.h"
#include <d3dx12.h>
#include "throw_if_failed.h"
#include "vertex.h"
#include <DirectXColors.h>
#include <iostream>

ComPtr<ID3D12Resource> d3dUtil::CreateDefaultBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* CmdList,
	const void* initData, UINT64 byteSize, ComPtr<ID3D12Resource>& uploadBuffer) 
{
	ComPtr<ID3D12Resource> defaultBuffer;
	CD3DX12_HEAP_PROPERTIES heapPr = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	CD3DX12_RESOURCE_DESC bs = CD3DX12_RESOURCE_DESC::Buffer(byteSize);
	ThrowIfFailed(device->CreateCommittedResource(&heapPr, D3D12_HEAP_FLAG_NONE, &bs,
		D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(defaultBuffer.GetAddressOf())));
	heapPr = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	ThrowIfFailed(device->CreateCommittedResource(&heapPr, D3D12_HEAP_FLAG_NONE, &bs,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(uploadBuffer.GetAddressOf())));

	D3D12_SUBRESOURCE_DATA subResourceData = {};
	subResourceData.pData = initData;
	subResourceData.RowPitch = byteSize;
	subResourceData.SlicePitch = subResourceData.RowPitch;

	CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);
	CmdList->ResourceBarrier(1, &barrier);
	UpdateSubresources<1>(CmdList, defaultBuffer.Get(), uploadBuffer.Get(), 0, 0, 1, &subResourceData);
	barrier = CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);
	CmdList->ResourceBarrier(1, &barrier);
	return defaultBuffer;
}

UINT d3dUtil::CalcConstantBufferSize(UINT byteSize) {
	return (byteSize + 255) & ~255;
}

