#pragma once

#include <DXF.h>

using namespace std;
using namespace DirectX;

class GreyScaleFilter : public BaseShader
{
private:
	struct LightBufferType
	{
		XMFLOAT4 diffuse;
		XMFLOAT3 direction;
		float padding;
	};

public:
	GreyScaleFilter(ID3D11Device* device, HWND hwnd);
	~GreyScaleFilter();

	void setShaderParameters(ID3D11DeviceContext* deviceContext,
		const XMMATRIX& world,
		const XMMATRIX& view,
		const XMMATRIX& projection,
		ID3D11ShaderResourceView* texture,
		Light* light);

private:
	void initShader(const wchar_t* vs, const wchar_t* ps);

private:
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11Buffer* lightBuffer;
};

