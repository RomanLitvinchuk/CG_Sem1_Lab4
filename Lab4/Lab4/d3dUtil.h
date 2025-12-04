#ifndef D3DUTIL_H_
#define D3DUTIL_H_
#include <d3d12.h>
#include <wrl.h>

using namespace Microsoft::WRL;

class d3dUtil {
public:
	static ComPtr<ID3D12Resource> CreateDefaultBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* CmdList,
		const void* initData, UINT64 byteSize, ComPtr<ID3D12Resource>& uploadBuffer);
	void CreateVertexBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* CmdList);

private:
	ComPtr<ID3D12Resource> VertexBufferGPU_ = nullptr;
	ComPtr<ID3D12Resource> VertexBufferUploader_ = nullptr;
};


#endif //D3DUTIL_H_
