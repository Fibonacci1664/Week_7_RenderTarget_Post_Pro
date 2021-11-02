// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "App1.h"

App1::App1()
{
	/*BEcameraXPos = 25.0f;
	BEcameraYPos = 60.0f;
	BEcameraZPos = 25.0f;*/
	playerPos.x = 25.0f;
	playerPos.y = 0.0f;
	playerPos.z = 25.0f;
	playerPos.w = 1.0f;
}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in, bool VSYNC, bool FULL_SCREEN)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in, VSYNC, FULL_SCREEN);
	screenSize->x = screenWidth;
	screenSize->y = screenHeight;
	initPostProKernels();

	/*BEcamera = new Camera();
	BEcamera->setPosition(BEcameraXPos, BEcameraYPos, BEcameraZPos);
	BEcamera->setRotation(90.0f, 0.0f, 0.0f);
	BEcamera->update();*/

	camera->setPosition(camera->getPosition().x + 5.0f, camera->getPosition().y, camera->getPosition().z);
	camera->setRotation(camera->getRotation().x, camera->getRotation().y - 25.0f, camera->getRotation().z);
	

	// Create Mesh object and shader object
	// Cube mesh is the geometry in the scene.
	// Ortho mesh is the geometry we render the result to.
	textureMgr->loadTexture(L"brick", L"res/brick1.dds");
	cubeMesh = new CubeMesh(renderer->getDevice(), renderer->getDeviceContext());
	//camerPosMesh = new CubeMesh(renderer->getDevice(), renderer->getDeviceContext());
	//sphereMesh = new SphereMesh(renderer->getDevice(), renderer->getDeviceContext());
	//planeMesh = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext(), 50);

	int orthoMeshWidth = screenWidth / 4;
	int orthoMeshHeight = screenHeight / 4;

	//orthoMesh_1 = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), orthoMeshWidth, orthoMeshHeight, (-screenWidth * 0.5f) + (orthoMeshWidth * 0.5f), (screenHeight * 0.5f) - (orthoMeshHeight * 0.5f));
	// For RH corner window
	//orthoMesh_2 = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), orthoMeshWidth, orthoMeshHeight, (screenWidth * 0.5f) - (orthoMeshWidth * 0.5f), (screenHeight * 0.5f) - (orthoMeshHeight * 0.5f));

	// Full screen orthomesh
	orthoMesh_1 = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), screenWidth, screenHeight, (-screenWidth * 0.5f) + (screenWidth * 0.5f), (screenHeight * 0.5f) - (screenHeight * 0.5f));
	//orthoMesh_2 = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), screenWidth, screenHeight, (-screenWidth * 0.5f) + (screenWidth * 0.5f), (screenHeight * 0.5f) - (screenHeight * 0.5f));

	// Initialise shaders
	// Lighting for normal rendering
	// Texture to output the result (we don't need to do lighting again, as it has already be calculated)
	lightShader = new LightShader(renderer->getDevice(), hwnd);
	textureShader = new TextureShader(renderer->getDevice(), hwnd);
	blurShader = new BlurShader(renderer->getDevice(), hwnd);
	/*colourShader = new ColourShader(renderer->getDevice(), hwnd);
	greyScaleShader = new GreyScaleFilter(renderer->getDevice(), hwnd);
	mmShader = new MiniMapShader(renderer->getDevice(), hwnd);*/

	// Build RenderTexture, this will be our alternative render target.
	renderTexture_1 = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
	renderTexture_2 = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);

	// Configure directional light
	light = new Light();
	light->setAmbientColour(0.0f, 0.0f, 0.0f, 1.0f);
	light->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	light->setDirection(0.7f, -0.7f, 0.7f);
}


App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D object.
	if (cubeMesh != nullptr)
	{
		delete cubeMesh;
	}

	if (camerPosMesh)
	{
		delete camerPosMesh;
		camerPosMesh = nullptr;
	}

	if (sphereMesh)
	{
		delete sphereMesh;
		sphereMesh = nullptr;
	}

	if (planeMesh)
	{
		delete planeMesh;
		planeMesh = nullptr;
	}

	if (orthoMesh_1)
	{
		delete orthoMesh_1;
		orthoMesh_1 = nullptr;
	}

	if (orthoMesh_2)
	{
		delete orthoMesh_2;
		orthoMesh_2 = nullptr;
	}

	if (lightShader)
	{
		delete lightShader;
		lightShader = nullptr;
	}

	if (textureShader)
	{
		delete textureShader;
		textureShader = nullptr;
	}

	if (colourShader)
	{
		delete colourShader;
		colourShader = nullptr;
	}

	if (greyScaleShader)
	{
		delete greyScaleShader;
		greyScaleShader = nullptr;
	}
	
	if (mmShader)
	{
		delete mmShader;
		mmShader = nullptr;
	}

	if (blurShader)
	{
		delete blurShader;
		blurShader = nullptr;
	}

	if (renderTexture_1)
	{
		delete renderTexture_1;
		renderTexture_1 = nullptr;
	}

	if (renderTexture_2)
	{
		delete renderTexture_2;
		renderTexture_2 = nullptr;
	}
}

bool App1::frame()
{
	bool result;

	result = BaseApplication::frame();

	if (!result)
	{
		return false;
	}

	//moveBECamera();
	//movePlayer();

	// Birds eye cam
	//BEcamera->setPosition(BEcameraXPos, BEcameraYPos, BEcameraZPos);
	//BEcamera->setPosition(playerPos.x, BEcameraYPos, playerPos.z);

	// Basic follow cam
	//camera->setPosition(playerPos.x, playerPos.y + 5.0f, playerPos.z - 10.0f);
	
	// Render the graphics.
	result = render();

	if (!result)
	{
		return false;
	}

	return true;
}


void App1::initPostProKernels()
{
	/*float boxBlurKernelArr[16] = {	1.0f, 1.0f, 1.0f, 0.0f,
								1.0f, 1.0f, 1.0f, 0.0f,
								1.0f, 1.0f, 1.0f, 0.0f,
								0.0f, 0.0f, 0.0f, 0.0f };*/

	XMFLOAT4 boxBlurRow1 = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);
	XMFLOAT4 boxBlurRow2 = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);
	XMFLOAT4 boxBlurRow3 = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);
	XMFLOAT4 boxBlurRow4 = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	boxBlurKernel[0] = boxBlurRow1;
	boxBlurKernel[1] = boxBlurRow2;
	boxBlurKernel[2] = boxBlurRow3;
	boxBlurKernel[3] = boxBlurRow4;
	boxBlurNeighbours = 9.0f;

	float gaussianBlurKernelArr[16] = { 1.0f, 1.0f, 1.0f, 0.0f,
									 1.0f, 1.0f, 1.0f, 0.0f,
									 1.0f, 1.0f, 1.0f, 0.0f,
									 0.0f, 0.0f, 0.0f, 0.0f };

	gaussianBlurKernel = XMFLOAT4X4(gaussianBlurKernelArr);
	gaussianBlurNeighbours = 1 / 16.0f;

	float edgeKernelArr[16] = { 1.0f, 1.0f, 1.0f, 0.0f,
							 1.0f, 1.0f, 1.0f, 0.0f,
							 1.0f, 1.0f, 1.0f, 0.0f,
							 0.0f, 0.0f, 0.0f, 0.0f };

	edgeKernel = XMFLOAT4X4(edgeKernelArr);
	edgeNeighbours = 1.0f;
}

void App1::movePlayer()
{
	// Move player left
	if (input->isKeyDown('F'))
	{
		--playerPos.x;
	}

	// Move player right
	if (input->isKeyDown('H'))
	{
		++playerPos.x;
	}

	// Move player fwd
	if (input->isKeyDown('T'))
	{
		++playerPos.z;
	}

	// Move player back
	if (input->isKeyDown('G'))
	{
		--playerPos.z;
	}
}

void App1::moveBECamera()
{
	// Move birds eye camera left
	if (input->isKeyDown('U'))
	{
		--BEcameraXPos;
	}

	// Move birds eye camera right
	if (input->isKeyDown('O'))
	{
		++BEcameraXPos;
	}

	// Move birds eye camera fwd (along z)
	if (input->isKeyDown('P'))
	{
		++BEcameraZPos;
	}

	// Move birds eye camera bckwrds (along z)
	if (input->isKeyDown('L'))
	{
		--BEcameraZPos;
	}

	// Move birds eye camera up
	if (input->isKeyDown('I'))
	{
		++BEcameraYPos;
	}

	// Move birds eye camera down
	if (input->isKeyDown('K'))
	{
		--BEcameraYPos;
	}
}

bool App1::render()
{
	// Render first pass to render texture
	firstPass();

	secondPass();

	// Render final pass to frame buffer
	finalPass();

	return true;
}

void App1::firstPass()
{
	// Set the render target to be the render to texture and clear it
	renderTexture_1->setRenderTarget(renderer->getDeviceContext());
	renderTexture_1->clearRenderTarget(renderer->getDeviceContext(), 1.0f, 0.2f, 0.2f, 1.0f);

	// Get matrices
	camera->update();
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX viewMatrix = camera->getViewMatrix();
	XMMATRIX projectionMatrix = renderer->getProjectionMatrix();
	
	// Render shape with simple lighting shader set.
	cubeMesh->sendData(renderer->getDeviceContext());
	lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"brick"), light);
	lightShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount());

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	renderer->setBackBufferRenderTarget();
}

void App1::secondPass()
{
	// Set the render target to be the render to texture and clear it
	renderTexture_2->setRenderTarget(renderer->getDeviceContext());
	renderTexture_2->clearRenderTarget(renderer->getDeviceContext(), 0.2, 0.2f, 0.2f, 1.0f);

	// Get matrices
	//BEcamera->setPosition(playerPos.x, BEcameraYPos, playerPos.z);
	camera->update();
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX viewMatrix = camera->getViewMatrix();
	XMMATRIX projectionMatrix = renderer->getProjectionMatrix();

	renderer->setZBuffer(false);
	XMMATRIX orthoViewMatrix = camera->getOrthoViewMatrix();	// Default camera position for orthographic rendering
	XMMATRIX orthoMatrix = renderer->getOrthoMatrix();			// ortho matrix for 2D rendering

	orthoMesh_1->sendData(renderer->getDeviceContext());

	XMFLOAT4 test = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);

	// Use a custom blur shader here
	blurShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix, renderTexture_1->getShaderResourceView(), screenSize, &test, boxBlurNeighbours);
	blurShader->render(renderer->getDeviceContext(), orthoMesh_1->getIndexCount());
	renderer->setZBuffer(true);

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	renderer->setBackBufferRenderTarget();
}

void App1::finalPass()
{
	// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	// Get matrices
	camera->update();
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX viewMatrix = camera->getViewMatrix();
	XMMATRIX projectionMatrix = renderer->getProjectionMatrix();

	// Render a cubiod mesh to represent the location of the player in the world
	/*worldMatrix *= XMMatrixTranslation(0, 0.9f, 0);
	worldMatrix *= XMMatrixScaling(0.2f, 1.0f, 0.2f);*/
	//worldMatrix *= XMMatrixTranslation(playerPos.x, playerPos.y, playerPos.z);
	cubeMesh->sendData(renderer->getDeviceContext());
	lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"brick"), light);
	lightShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount());

	// Render a cubiod mesh to represent the location of the birds eye camera in the world
	/*worldMatrix = XMMatrixIdentity();
	worldMatrix *= XMMatrixScaling(0.2f, 0.6f, 0.2f);
	worldMatrix *= XMMatrixTranslation(BEcameraXPos, BEcameraYPos, BEcameraZPos);
	camerPosMesh->sendData(renderer->getDeviceContext());
	lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"brick"), light);
	lightShader->render(renderer->getDeviceContext(), camerPosMesh->getIndexCount());*/

	// The ground
	/*worldMatrix = XMMatrixIdentity();
	planeMesh->sendData(renderer->getDeviceContext());
	lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"brick"), light);
	lightShader->render(renderer->getDeviceContext(), planeMesh->getIndexCount());*/
	
	/*worldMatrix *= XMMatrixTranslation(2.0f, 0.0f, 5.0f);
	sphereMesh->sendData(renderer->getDeviceContext());
	lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"brick"), light);
	lightShader->render(renderer->getDeviceContext(), sphereMesh->getIndexCount());*/

	// RENDER THE RENDER TEXTURE SCENE
	// Requires 2D rendering and an ortho mesh.
	renderer->setZBuffer(false);
	XMMATRIX orthoViewMatrix = camera->getOrthoViewMatrix();	// Default camera position for orthographic rendering
	XMMATRIX orthoMatrix = renderer->getOrthoMatrix();			// ortho matrix for 2D rendering

	orthoMesh_1->sendData(renderer->getDeviceContext());
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix, renderTexture_2->getShaderResourceView());
	textureShader->render(renderer->getDeviceContext(), orthoMesh_1->getIndexCount());

	/*orthoMesh_2->sendData(renderer->getDeviceContext());
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix, renderTexture_2->getShaderResourceView());
	textureShader->render(renderer->getDeviceContext(), orthoMesh_2->getIndexCount());*/
	renderer->setZBuffer(true);

	// Render GUI
	gui();

	// Present the rendered scene to the screen.
	renderer->endScene();
}

void App1::gui()
{
	// Force turn off unnecessary shader stages.
	renderer->getDeviceContext()->GSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->HSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->DSSetShader(NULL, NULL, 0);

	// Build UI
	ImGui::Text("FPS: %.2f", timer->getFPS());
	ImGui::Checkbox("Wireframe mode", &wireframeToggle);

	// Render UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

