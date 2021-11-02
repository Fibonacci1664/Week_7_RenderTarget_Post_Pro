#include "BlurShader.h"

BlurShader::BlurShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	screenBuffer = nullptr;

	initShader(L"blur_vs.cso", L"blur_ps.cso");
}

BlurShader::~BlurShader()
{
	// Release the sampler state.
	if (sampleState)
	{
		sampleState->Release();
		sampleState = 0;
	}

	// Release the matrix constant buffer.
	if (screenBuffer)
	{
		screenBuffer->Release();
		screenBuffer = 0;
	}

	if (matrixBuffer)
	{
		matrixBuffer->Release();
		matrixBuffer = 0;
	}

	// Release the layout.
	if (layout)
	{
		layout->Release();
		layout = 0;
	}

	//Release base shader components
	BaseShader::~BaseShader();
}


void BlurShader::initShader(const wchar_t* vsFilename, const wchar_t* psFilename)
{
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC screenBufferDesc;
	D3D11_BUFFER_DESC kernelBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;

	// Load (+ compile) shader files
	loadVertexShader(vsFilename);
	loadPixelShader(psFilename);

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	renderer->CreateBuffer(&matrixBufferDesc, NULL, &matrixBuffer);

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	renderer->CreateSamplerState(&samplerDesc, &sampleState);

	screenBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	screenBufferDesc.ByteWidth = sizeof(ScreenBufferType);
	screenBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	screenBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	screenBufferDesc.MiscFlags = 0;
	screenBufferDesc.StructureByteStride = 0;
	renderer->CreateBuffer(&screenBufferDesc, NULL, &screenBuffer);

	kernelBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	kernelBufferDesc.ByteWidth = sizeof(KernelBufferType);
	kernelBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	kernelBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	kernelBufferDesc.MiscFlags = 0;
	kernelBufferDesc.StructureByteStride = 0;
	renderer->CreateBuffer(&kernelBufferDesc, NULL, &kernelBuffer);
}

void BlurShader::setShaderParameters(ID3D11DeviceContext* deviceContext,
	const XMMATRIX& worldMatrix,
	const XMMATRIX& viewMatrix,
	const XMMATRIX& projectionMatrix,
	ID3D11ShaderResourceView* texture,
	XMFLOAT2* screenSz,
	XMFLOAT4* kernel,
	float nhSize)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	XMMATRIX tworld, tview, tproj;

	// Transpose the matrices to prepare them for the shader.
	tworld = XMMatrixTranspose(worldMatrix);
	tview = XMMatrixTranspose(viewMatrix);
	tproj = XMMatrixTranspose(projectionMatrix);

	// Send matrix data
	result = deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	dataPtr = (MatrixBufferType*)mappedResource.pData;
	dataPtr->world = tworld;// worldMatrix;
	dataPtr->view = tview;
	dataPtr->projection = tproj;
	deviceContext->Unmap(matrixBuffer, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &matrixBuffer);

	ScreenBufferType* screenPtr;
	deviceContext->Map(screenBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	screenPtr = (ScreenBufferType*)mappedResource.pData;
	screenPtr->screenSize = screenSz;
	screenPtr->padding = XMFLOAT2(0.0f, 0.0f);
	deviceContext->Unmap(screenBuffer, 0);
	deviceContext->PSSetConstantBuffers(0, 1, &screenBuffer);

	KernelBufferType* kernelPtr;
	deviceContext->Map(screenBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	kernelPtr = (KernelBufferType*)mappedResource.pData;
	kernelPtr->kernel = kernel;
	/*kernelPtr->kernel[1] = kernel[1];
	kernelPtr->kernel[2] = kernel[2];
	kernelPtr->kernel[3] = kernel[3];*/
	kernelPtr->neighbourhoodSize = nhSize;
	kernelPtr->nhPadding = XMFLOAT3(0.0f, 0.0f, 0.0f);
	deviceContext->Unmap(kernelBuffer, 0);
	deviceContext->PSSetConstantBuffers(1, 1, &kernelBuffer);

	// Set shader texture and sampler resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);
	deviceContext->PSSetSamplers(0, 1, &sampleState);
}





