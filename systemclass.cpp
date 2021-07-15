#include "systemclass.h"

SystemClass::SystemClass() {
	m_Input = 0;
	m_Graphics = 0;
}

SystemClass::SystemClass(const SystemClass& other) {

}

SystemClass::~SystemClass() {

}

bool SystemClass::Initialize() {
	int screenWidth, screenHeight;	

	screenWidth = 0;
	screenHeight = 0;
	
	InitializeWindows(screenWidth, screenHeight);
	
	m_Input = new InputClass();
	if (!m_Input) {
		return false;
	}
	m_Input->Initialize();

	m_Graphics = new GraphicsClass();
	if (!m_Graphics) {
		return false;
	}
	
	if (!(m_Graphics->Initialize(screenWidth, screenHeight, m_hWnd))) {
		return false;
	}
	return true;
}

void SystemClass::Shutdown() {

	if (m_Graphics) {
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	if (m_Input) {
		delete m_Input;
		m_Input = 0;
	}

	ShutdownWindows();

	return;
}

void SystemClass::Run() {
	MSG msg;
	bool done;

	ZeroMemory(&msg, sizeof(MSG));

	done = false;

	while (!done) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}

		if (msg.message == WM_QUIT) {
			done = true;
		}
		else {			
			if (!Frame()) {
				done = true;
			}
		}
	}

	return;
}

bool SystemClass::Frame() {
	if (m_Input->IsKeyDown(VK_ESCAPE)) {		
		return false;
	}

	if (!(m_Graphics->Frame())) {
		return false;
	}
	return true;
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_KEYDOWN: {
			m_Input->KeyDown((unsigned int)wParam);
			return 0;
		}
		case WM_KEYUP: {
			m_Input->KeyUp((unsigned int)wParam);
			return 0;
		}
		default: {
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
	}
}

void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight) {
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	applicationHandle = this;
	m_hInstance = GetModuleHandle(NULL);
	m_applicationName = L"Engine";
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	screenWidth  = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	if (FULL_SCREEN) {
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		posX = posY = 0;
	}
	else {
		screenWidth = 1366;
		screenHeight = 768;
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	m_hWnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP, posX, posY, screenWidth, screenHeight, NULL, NULL, m_hInstance, NULL);
	ShowWindow(m_hWnd, SW_SHOW);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);

	ShowCursor(false);

	return;
}

void SystemClass::ShutdownWindows() {
	ShowCursor(true);

	if (FULL_SCREEN) {
		ChangeDisplaySettings(NULL, 0);
	}

	DestroyWindow(m_hWnd);
	m_hWnd = NULL;
	UnregisterClass(m_applicationName, m_hInstance);
	m_hInstance = NULL;

	applicationHandle = NULL;

	return;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam) {
	switch (uMessage) {
		case WM_DESTROY: {
			PostQuitMessage(0);
			return 0;
		}
		case WM_CLOSE: {
			PostQuitMessage(0);
			return 0;
		}
		default: {
			return applicationHandle->MessageHandler(hWnd, uMessage, wParam, lParam);
		}
	}
}