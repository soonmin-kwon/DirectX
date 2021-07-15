#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>
#include <iostream>
#include "textureclass.h"

class ModelClass {
public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*);
	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*);
	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*, char*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

private:
	/*struct VertexType {
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};*/

	struct VertexType {
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
		DirectX::XMFLOAT3 normal;
	};

	struct ModelType {
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
	bool LoadModel(char*);	
	void ReleaseTexture();
	void ReleaseModel();

	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;
	int m_vertexCount, m_indexCount;
	TextureClass* m_texture;
	ModelType* m_model;
};

#endif // !_MODELCLASS_H_