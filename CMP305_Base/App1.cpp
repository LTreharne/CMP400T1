#include "App1.h"

App1::App1()
{
	m_Terrain = nullptr;
	shader = nullptr;
}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in, bool VSYNC, bool FULL_SCREEN)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in, VSYNC, FULL_SCREEN);

	// Load textures
	textureMgr->loadTexture(L"grass", L"res/grass.png");
	textureMgr->loadTexture(L"white", L"res/DefaultDiffuse.png");

	lithosphere.AddPlate(XMINT4(16, 24, 50, 30));

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
	shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"grass"), light);
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
	
	Plate pla = lithosphere.plates[0];

	XMINT4 plateProperties = XMINT4(pla.width, pla.height, pla.xOff, pla.yOff);

	if (ImGui::DragInt4("Plate 0",&plateProperties.x,1, 0,127, "%i")) {
		lithosphere.plates[0].UpdateProperties(plateProperties);
		lithosphere.GenerateHeightMap();
		m_Terrain->Regenerate(renderer->getDevice(), renderer->getDeviceContext(), lithosphere.lithoHeightMap);
	}

	Plate pla1 = lithosphere.plates[1];

	XMINT4 plateProperties1 = XMINT4(pla1.width, pla1.height, pla1.xOff, pla1.yOff);

	if (ImGui::DragInt4("Plate 1", &plateProperties1.x, 1, 0, 127, "%i")) {
		lithosphere.plates[1].UpdateProperties(plateProperties1);
		lithosphere.GenerateHeightMap();
		m_Terrain->Regenerate(renderer->getDevice(), renderer->getDeviceContext(), lithosphere.lithoHeightMap);
	}


	//ImGui::SliderInt("Terrain Resolution", &terrainResolution, 2, 1024);

	if (ImGui::Button("Regenerate Terrain")) {
		if (terrainResolution != m_Terrain->GetResolution()) {
			m_Terrain->Resize(terrainResolution);
		}
		m_Terrain->Regenerate(renderer->getDevice(), renderer->getDeviceContext(), lithosphere.lithoHeightMap);
	}

	// Render UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

