#ifndef _GRAPHICSCLASS_H_ 
#define _GRAPHICSCLASS_H_

#include "d3dclass.h"
#include "cameraclass.h"
#include "textureshaderclass.h"
//#include "modelclass.h"
//#include "lightshaderclass.h"
//#include "lightclass.h"
#include "bitmapclass.h"

//#include "colorshaderclass.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1F;

class GraphicsClass {
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

private:
	bool Render();
	bool Render(float);

	D3DClass* m_direct3D;
	CameraClass* m_camera;
	//ModelClass* m_model;
	//LightShaderClass* m_lightShader;
	//LightClass* m_light;
	//ColorShaderClass* m_colorShader;
	TextureShaderClass* m_textureShader;
	BitmapClass* m_bitmap;
};
#endif