#pragma once

#include "DXF.h"

using namespace std;
using namespace DirectX;

class BlurShader : public BaseShader
{
private:
	struct ScreenBufferType
	{
		float scrWidth;
		float scrHeight;
		XMFLOAT2 padding;
	};

	struct KernelBufferType
	{
		XMMATRIX kernel;
		float kernelCoefficient;
		XMFLOAT3 coeffPadding;
	};

public:
	BlurShader(ID3D11Device* device, HWND hwnd);
	~BlurShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext,
		const XMMATRIX& world,
		const XMMATRIX& view,
		const XMMATRIX& projection,
		ID3D11ShaderResourceView* texture,
		float sWidth,
		float sHeight,
		XMMATRIX& kernel,
		float kernelCoeff);

private:
	void initShader(const wchar_t* vs, const wchar_t* ps);

private:
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11Buffer* screenBuffer;
	ID3D11Buffer* kernelBuffer;
};

