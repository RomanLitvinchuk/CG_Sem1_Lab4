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

void d3dUtil::CreateVertexBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* CmdList) {
	Vertex vertices[] =
	{
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::White) },
		{ XMFLOAT3(-1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Black) },
		{ XMFLOAT3(+1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Red) },
		{ XMFLOAT3(+1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::Green) },
		{ XMFLOAT3(-1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Blue) },
		{ XMFLOAT3(-1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Yellow) },
		{ XMFLOAT3(+1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Cyan) },
		{ XMFLOAT3(+1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Magenta) }
	};
	UINT vbByteSize = 8 * sizeof(Vertex);
	VertexBufferGPU_ = d3dUtil::CreateDefaultBuffer(device, CmdList, vertices, vbByteSize, VertexBufferUploader_);
	D3D12_VERTEX_BUFFER_VIEW vbv;
	vbv.BufferLocation = VertexBufferGPU_->GetGPUVirtualAddress();
	vbv.SizeInBytes = vbByteSize;
	vbv.StrideInBytes = sizeof(Vertex);
	D3D12_VERTEX_BUFFER_VIEW VertexBuffers[1] = { vbv };
	CmdList->IASetVertexBuffers(0, 1, VertexBuffers);
	std::cout << "Vertex Buffer is set" << std::endl;
}

void d3dUtil::CreateIndexBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* CmdList) {
	std::uint16_t indices[] = {
		// front face
		0, 1, 2,
		0, 2, 3,
		// back face
		4, 6, 5,
		4, 7, 6,
		// left face
		4, 5, 1,
		4, 1, 0,
		// right face
		3, 2, 6,
		3, 6, 7,
		// top face
		1, 5, 6,
		1, 6, 2,
		// bottom face
		4, 0, 3,
		4, 3, 7
	};
	UINT ibByteSize = 36 * sizeof(std::uint16_t);
	IndexBufferGPU_ = d3dUtil::CreateDefaultBuffer(device, CmdList, indices, ibByteSize, IndexBufferUploader_);
	D3D12_INDEX_BUFFER_VIEW ibv;
	ibv.BufferLocation = IndexBufferGPU_->GetGPUVirtualAddress();
	ibv.SizeInBytes = ibByteSize;
	ibv.Format = DXGI_FORMAT_R16_UINT;
	CmdList->IASetIndexBuffer(&ibv);
	std::cout << "Index buffer is set" << std::endl;
}