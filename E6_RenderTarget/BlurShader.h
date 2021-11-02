#pragma once

#include "DXF.h"

using namespace std;
using namespace DirectX;

class BlurShader : public BaseShader
{
private:
	struct ScreenBufferType
	{
		XMFLOAT2 screenSize;
		XMFLOAT2 padding;
	};

	struct KernelBufferType
	{
		XMFLOAT4X4 kernel;
		float coeff;
		XMFLOAT3 nhPadding;
	};

public:
	BlurShader(ID3D11Device* device, HWND hwnd);
	~BlurShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext,
		const XMMATRIX& world,
		const XMMATRIX& view,
		const XMMATRIX& projection,
		ID3D11ShaderResourceView* texture,
		XMFLOAT2 screenSz,
		XMFLOAT4X4 kernel,
		float coeff);

private:
	void initShader(const wchar_t* vs, const wchar_t* ps);

private:
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11Buffer* screenBuffer;
	ID3D11Buffer* kernelBuffer;
};

