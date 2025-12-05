#ifndef D3DUTIL_H_
#define D3DUTIL_H_
#include <d3d12.h>
#include <wrl.h>
#include <SimpleMath.h>

using namespace Microsoft::WRL;
using namespace SimpleMath;

struct cbPerObject {
	Matrix mWorldViewProj;
};


class d3dUtil {
public:
	static ComPtr<ID3D12Resource> CreateDefaultBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* CmdList,
		const void* initData, UINT64 byteSize, ComPtr<ID3D12Resource>& uploadBuffer);
	void CreateVertexBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* CmdList);
	void CreateIndexBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* CmdList);
	static UINT CalcConstantBufferSize(UINT byteSize);
private:
	ComPtr<ID3D12Resource> VertexBufferGPU_ = nullptr;
	ComPtr<ID3D12Resource> VertexBufferUploader_ = nullptr;

	ComPtr<ID3D12Resource> IndexBufferGPU_ = nullptr;
	ComPtr<ID3D12Resource> IndexBufferUploader_ = nullptr;
};


#endif //D3DUTIL_H_
