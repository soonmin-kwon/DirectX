#include "graphicsclass.h"
#include <float.h>
using namespace DirectX;
using namespace std;

GraphicsClass::GraphicsClass() {
	m_direct3D = 0;
	m_camera = 0;
	//m_model = 0;
	//m_colorShader = 0;
	m_textureShader = 0;
	//m_light = 0;
	//m_lightShader = 0;
	m_bitmap = 0;
}

GraphicsClass::GraphicsClass(const GraphicsClass& other) {
}

GraphicsClass::~GraphicsClass() {
}

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hWnd) {
	bool result;

	m_direct3D = new D3DClass();
	if (!m_direct3D) {
		return false;
	}
	result = m_direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hWnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result) {
		MessageBox(hWnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	m_camera = new CameraClass();
	if (!m_camera) {
		return false;
	}	
	m_camera->SetPosition(0.0f, 0.0f, -10.0f);

	/*m_model = new ModelClass();
	if (!m_model) {
		return false;
	}
	result = m_model->Initialize(m_direct3D->GetDevice(), m_direct3D->GetDeviceContext(), (char*)"../DirectX/data/cube.txt", (char*)"../DirectX/data/stone01.tga");
	if (!result) {
		MessageBox(hWnd, L"Could not initialize the model obj", L"Error", MB_OK);
		return false;
	}	

	m_lightShader = new LightShaderClass();
	if (!m_lightShader) {
		return false;
	}
	result = m_lightShader->Initialize(m_direct3D->GetDevice(), hWnd);
	if (!result) {
		MessageBox(hWnd, L"Could not initialize the light shader object", L"Error", MB_OK);
		return false;
	}

	m_light = new LightClass();
	if (!m_light) {
		return false;
	}
		
	m_light->SetAmbientColor(0.150f, 0.150f, 0.150f, 1.0f);
	m_light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_light->SetDirection(0.0f, 0.0f, 1.0f);
	m_light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_light->SetSpecularPower(32.0f);
	

	m_colorShader = new ColorShaderClass();
	if (!m_colorShader) {
		return false;
	}
	result = m_colorShader->Initialize(m_direct3D->GetDevice(), hWnd);
	if (!result) {
		MessageBox(hWnd, L"Could not initialize the color shader obj", L"Error", MB_OK);
		return false;
	}*/

	m_textureShader = new TextureShaderClass();
	if (!m_textureShader) {
		return false;
	}
	result = m_textureShader->Initialize(m_direct3D->GetDevice(), hWnd);
	if (!result) {
		MessageBox(hWnd, L"Could not initialize the color shader obj", L"Error", MB_OK);
		return false;
	}

	m_bitmap = new BitmapClass();
	if (!m_bitmap) {
		return false;
	}
	result = m_bitmap->Initialize(m_direct3D->GetDevice(), m_direct3D->GetDeviceContext(), (char*)"../DirectX/data/stone01.tga", screenWidth, screenHeight, 256, 256);
	if (!result) {
		MessageBoxW(hWnd, L"Could not initialize the bitmap object", L"Error", MB_OK);
		return false;
	}

	return true;
}

void GraphicsClass::Shutdown() {
	/*if (m_light) {
		delete m_light;
		m_light = 0;
	}

	if (m_lightShader) {
		m_lightShader->Shutdown();
		delete m_lightShader;
		m_lightShader = 0;
	}

	if (m_model) {
		m_model->Shutdown();
		delete m_model;
		m_model = 0;
	}*/

	if (m_bitmap) {
		m_bitmap->Shutdown();
		delete m_bitmap;
		m_bitmap = 0;
	}

	if (m_camera) {
		delete m_camera;
		m_camera = 0;
	}

	if (m_direct3D) {
		m_direct3D->Shutdown();
		delete m_direct3D;
		m_direct3D = 0;
	}
	
	/*if (m_colorShader) {
		m_colorShader->Shutdown();
		delete m_colorShader;
		m_colorShader = 0;
	}
	*/

	if (m_textureShader) {
		m_textureShader->Shutdown();
		delete m_textureShader;
		m_textureShader = 0;
	}

	return;
}

bool GraphicsClass::Frame() {	
	
	static float rotation = 0.0f;

	rotation += (float)XM_PI * 0.001f;
	if (rotation > 360.0f) {
		rotation -= 360.0f;
	}

	if (!Render(rotation)) {
		return false;
	}

	return true;
}

/*bool GraphicsClass::Render() {
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	m_direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);
	m_camera->Render();

	m_direct3D->GetWorldMatrix(worldMatrix);
	m_camera->GetViewMatrix(viewMatrix);
	m_direct3D->GetProjectionMatrix(projectionMatrix);

	m_model->Render(m_direct3D->GetDeviceContext());
	/*result = m_colorShader->Render(m_direct3D->GetDeviceContext(), m_model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result) {
		return false;
	}
	
	result = m_textureShader->Render(m_direct3D->GetDeviceContext(), m_model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_model->GetTexture());
	if (!result) {
		return false;
	}

	m_direct3D->EndScene();

	return true;
}*/

bool GraphicsClass::Render(float rotation) {
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	bool result;

	m_direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);
	m_camera->Render();

	m_camera->GetViewMatrix(viewMatrix);
	m_direct3D->GetWorldMatrix(worldMatrix);
	m_direct3D->GetProjectionMatrix(projectionMatrix);
	m_direct3D->GetOrthoMatrix(orthoMatrix);

	m_direct3D->TurnZBufferOff();

	result = m_bitmap->Render(m_direct3D->GetDeviceContext(), 300, 300);
	if (!result) {
		return false;
	}

	result = m_textureShader->Render(m_direct3D->GetDeviceContext(), m_bitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_bitmap->GetTexture());
	if (!result) {
		return false;
	}

	m_direct3D->TurnZBufferOn();

	//worldMatrix = XMMatrixRotationY(rotation);

	/*m_model->Render(m_direct3D->GetDeviceContext());

	result = m_lightShader->Render(m_direct3D->GetDeviceContext(), m_model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_model->GetTexture(), m_light->GetDirection(), m_light->GetDiffuseColor(), m_light->GetAmbientColor(), m_camera->GetPosition(), m_light->GetSpecularColor(), m_light->GetSpecularPower());
	if (!result) {
		return false;
	}*/

	m_direct3D->EndScene();

	return true;
}