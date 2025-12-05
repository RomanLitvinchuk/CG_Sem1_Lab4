#ifndef D3DUTIL_H_
#define D3DUTIL_H_
#include <d3d12.h>
#include <wrl.h>
#include <SimpleMath.h>

using namespace Microsoft::WRL;
using namespace DirectX;
using namespace DirectX::SimpleMath;


class d3dUtil {
public:
	static ComPtr<ID3D12Resource> CreateDefaultBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* CmdList,
		const void* initData, UINT64 byteSize, ComPtr<ID3D12Resource>& uploadBuffer);
	static UINT CalcConstantBufferSize(UINT byteSize);
private:
};


#endif //D3DUTIL_H_
