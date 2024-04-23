#include "App1.h"
#include <stdio.h>
#include <stdlib.h>
App1::App1()
{
	m_Terrain = nullptr;
	shader = nullptr;
}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in, bool VSYNC, bool FULL_SCREEN)
{
	srand(time(NULL));
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in, VSYNC, FULL_SCREEN);

	// Load textures
	textureMgr->loadTexture(L"grass", L"res/Grass.png");
	textureMgr->loadTexture(L"snow", L"res/snow.png");
	textureMgr->loadTexture(L"rock", L"res/rocks.png");
	textureMgr->loadTexture(L"water", L"res/water.png");
	textureMgr->loadTexture(L"white", L"res/DefaultDiffuse.png");

	lithosphere.GeneratePlates(25);
	//lithosphere.AddHotSpot(XMFLOAT4(100, 100, 10, 0.05), XMFLOAT2(0, 0));
	//lithosphere.AddHotSpot(XMFLOAT4(110, 122, 10, 0.1), XMFLOAT2(0, 0));
	//lithosphere.AddHotSpot(XMFLOAT4(135, 86, 10, 0.08), XMFLOAT2(0, 0));
	// Create Mesh object and shader object
	m_Terrain = new TerrainMesh(renderer->getDevice(), renderer->getDeviceContext());
	m_Terrain->Regenerate(renderer->getDevice(), renderer->getDeviceContext(), lithosphere.lithoHeightMap);
	shader = new LightShader(renderer->getDevice(), hwnd);
	
	
	


	// Initialise light
	light = new Light();
	light->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	light->setDirection(1.0f, -1.0f, 0.0f);

}


App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D object.
	if (m_Terrain)
	{
		delete m_Terrain;
		m_Terrain = 0;
	}

	if (shader)
	{
		delete shader;
		shader = 0;
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
	
	// Render the graphics.
	result = render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool App1::render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	// Generate the view matrix based on the camera's position.
	camera->update();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	worldMatrix = renderer->getWorldMatrix();
	viewMatrix = camera->getViewMatrix();
	projectionMatrix = renderer->getProjectionMatrix();

	// Send geometry data, set shader parameters, render object with shader
	m_Terrain->sendData(renderer->getDeviceContext());
	shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"grass"), textureMgr->getTexture(L"water"), textureMgr->getTexture(L"rock"), textureMgr->getTexture(L"snow"), light);
	shader->render(renderer->getDeviceContext(), m_Terrain->getIndexCount());

	// Render GUI
	gui();

	// Swap the buffers
	renderer->endScene();

	return true;
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
	

	if(lithosphere.plates.size() > 0){
	Plate pla = lithosphere.plates[0];

	XMINT4 plateProperties = XMINT4(pla.width, pla.height, pla.xOff, pla.yOff);

	if (ImGui::DragInt4("Plate 0",&plateProperties.x,1, 0, lithosphere.height, "%i")) {
		lithosphere.plates[0].UpdateProperties(plateProperties);
		lithosphere.GenerateHeightMap();
		m_Terrain->Regenerate(renderer->getDevice(), renderer->getDeviceContext(), lithosphere.lithoHeightMap);
	}

	ImGui::SliderFloat2("Plate 0 Speed", &lithosphere.plates[0].velocity.x, -sqrt(plateMaximumSpeed), sqrt(plateMaximumSpeed), "%.1f");
	}
	if (lithosphere.plates.size()>1) {


		Plate pla1 = lithosphere.plates[1];

		XMINT4 plateProperties1 = XMINT4(pla1.width, pla1.height, pla1.xOff, pla1.yOff);

		if (ImGui::DragInt4("Plate 1", &plateProperties1.x, 1, 0, lithosphere.height, "%i")) {
			lithosphere.plates[1].UpdateProperties(plateProperties1);
			lithosphere.GenerateHeightMap();
			m_Terrain->Regenerate(renderer->getDevice(), renderer->getDeviceContext(), lithosphere.lithoHeightMap);
		}

		ImGui::SliderFloat2("Plate 1 Speed", &lithosphere.plates[1].velocity.x, -sqrt(plateMaximumSpeed), sqrt(plateMaximumSpeed), "%.1f");
	}

	if (lithosphere.plates.size() > 2) {


		Plate pla2 = lithosphere.plates[2];

		XMINT4 plateProperties2 = XMINT4(pla2.width, pla2.height, pla2.xOff, pla2.yOff);

		if (ImGui::DragInt4("Plate 2", &plateProperties2.x, 1, 0, lithosphere.height, "%i")) {
			lithosphere.plates[2].UpdateProperties(plateProperties2);
			lithosphere.GenerateHeightMap();
			m_Terrain->Regenerate(renderer->getDevice(), renderer->getDeviceContext(), lithosphere.lithoHeightMap);
		}

		ImGui::SliderFloat2("Plate 2 Speed", &lithosphere.plates[2].velocity.x, -sqrt(plateMaximumSpeed), sqrt(plateMaximumSpeed), "%.1f");
	}


	//ImGui::SliderInt("Terrain Resolution", &terrainResolution, 2, 1024);

	if (ImGui::Button("Itterate")) {
		for (int i = 0; i < 100;++i) {
			lithosphere.Itterate();
		}
		m_Terrain->Regenerate(renderer->getDevice(), renderer->getDeviceContext(), lithosphere.lithoHeightMap);
	}
	if (ImGui::Button("Export")) {
		exportHeightmap();
	}


	// Render UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void App1::exportHeightmap()
{
	FILE* ImageFile;
	float maxHeight = 0;
	for (int i = 0; i < lithoWidth; ++i) {
		for (int j = 0; j < lithoWidth; ++j) {
			maxHeight = max(maxHeight, lithosphere.lithoHeightMap[i][j]);
		}
	}


	int pixelColour, height = lithoHeight, width = lithoWidth;

	ImageFile = fopen("image.pgm", "wb");
	if (ImageFile == NULL) {
		perror("ERROR: Cannot open output file");
		exit(EXIT_FAILURE);
	}

	fprintf(ImageFile, "P5\n");           // P5 filetype
	fprintf(ImageFile, "%d %d\n", width, height);   // dimensions
	fprintf(ImageFile, "255\n");          // Max pixel

	/* Now write a greyscale ramp */
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			pixelColour = (lithosphere.lithoHeightMap[j][i]/maxHeight)*256;
			fputc(pixelColour, ImageFile);
		}
	}

	fclose(ImageFile);
}

