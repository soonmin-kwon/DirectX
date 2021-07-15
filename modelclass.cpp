#include "modelclass.h"
using namespace DirectX;
using namespace std;

ModelClass::ModelClass() {
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_texture = 0;
	m_model = 0;
}

ModelClass::ModelClass(const ModelClass& other) {
}

ModelClass::~ModelClass() {
}

bool ModelClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* modelFileName, char* textureFileName) {
	if (!LoadModel(modelFileName)) {
		return false;
	}

	if (!InitializeBuffers(device)) {
		return false;
	}

	if (!LoadTexture(device, deviceContext, textureFileName)) {		
		return false;
	}	

	return true;
}
bool ModelClass::Initialize(ID3D11Device* device) {
	if (!InitializeBuffers(device)) {
		return false;
	}

	return true;
}

bool ModelClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* fileName) {
	if (!InitializeBuffers(device)) {
		return false;
	}

	if (!LoadTexture(device, deviceContext, fileName)) {
		return false;
	}

	return true;
}

void ModelClass::Shutdown() {
	ReleaseTexture();
	ShutdownBuffers();
	ReleaseModel();
	return;
}

void ModelClass::Render(ID3D11DeviceContext* deviceContext) {
	RenderBuffers(deviceContext);

	return;
}

int ModelClass::GetIndexCount() {
	return m_indexCount;
}

ID3D11ShaderResourceView* ModelClass::GetTexture() {
	return m_texture->GetTexture();
}

bool ModelClass::InitializeBuffers(ID3D11Device* device) {
	VertexType* vertices;	
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;
	D3D11_SUBRESOURCE_DATA indexData;
	HRESULT result;

	vertices = new VertexType[m_vertexCount];	
	if (!vertices) {		
		return false;
	}

	indices = new unsigned long[m_indexCount];
	if (!indices) {		
		return false;
	}

	for (int i = 0; i < m_vertexCount; i++) {
		vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = XMFLOAT2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		indices[i] = i;
	}

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result)) {		
		return false;
	}

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0; 
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result)) {
		cout << 1234 << "\n";
		return false;
	}
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void ModelClass::ShutdownBuffers() {
	if (m_indexBuffer) {
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}
	
	if (m_vertexBuffer) {
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}

void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext) {
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	

	return;
}

bool ModelClass::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* textureFileName) {	
	m_texture = new TextureClass();
	if (!m_texture) {
		return false;
	}

	if (!m_texture->Initialize(device, deviceContext, textureFileName)) {
		return false;
	}

	return true;
}

void ModelClass::ReleaseTexture() {
	if (m_texture) {
		m_texture->Shutdown();
		delete m_texture;
		m_texture = 0;
	}

	return;
}

bool ModelClass::LoadModel(char* modelFileName) {
	ifstream in;
	char input;

	in.open(modelFileName);
	if (in.fail()) {		
		return false;
	}
	in.get(input);

	while (input != ':') {
		in.get(input);
	}
	in >> m_vertexCount;

	m_indexCount = m_vertexCount;
	m_model = new ModelType[m_vertexCount];

	if (!m_model) {		
		return false;
	}

	in.get(input);
	while (input != ':') {
		in.get(input);
	}
	in.get(input);
	in.get(input);

	for (int i = 0; i < m_vertexCount; i++) {
		in >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		in >> m_model[i].tu >> m_model[i].tv;
		in >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}

	in.close();

	return true;
}

void ModelClass::ReleaseModel() {
	if (m_model) {
		delete[] m_model;
		m_model = 0;
	}

	return;
}