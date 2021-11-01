// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "DXF.h"	// include dxframework
#include "LightShader.h"
#include "TextureShader.h"
#include "ColourShader.h"
#include "GreyScaleFilter.h"
#include "MiniMapShader.h"
#include "BlurShader.h"

class App1 : public BaseApplication
{
public:

	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in, bool VSYNC, bool FULL_SCREEN);

	bool frame();

protected:
	bool render();
	void firstPass();
	void secondPass();
	void finalPass();
	void gui();

private:
	void moveBECamera();
	void movePlayer();
	void initPostProKernels();

	CubeMesh* cubeMesh;
	SphereMesh* sphereMesh;
	PlaneMesh* planeMesh;
	CubeMesh* camerPosMesh;
	
	OrthoMesh* orthoMesh_1;
	OrthoMesh* orthoMesh_2;
	LightShader* lightShader;
	TextureShader* textureShader;
	ColourShader* colourShader;
	GreyScaleFilter* greyScaleShader;
	MiniMapShader* mmShader;
	BlurShader* blurShader;

	RenderTexture* renderTexture_1;
	RenderTexture* renderTexture_2;

	Light* light;

	Camera* BEcamera;

	float BEcameraXPos;
	float BEcameraYPos;
	float BEcameraZPos;

	XMFLOAT4 playerPos;

	XMMATRIX boxBlurKernelMat;
	float boxBlurKernelCoeff;

	XMMATRIX gaussianBlurKernelMat;
	float gaussianBlurKernelCoeff;

	XMMATRIX edgeKernelMat;
	float edgeKernelCoeff;
};

#endif