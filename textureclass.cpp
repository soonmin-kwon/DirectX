#include "textureclass.h"

TextureClass::TextureClass(){
	m_targaData = 0;	
	m_texture2D = 0;	
	m_textureView = 0;
}

TextureClass::TextureClass(const TextureClass& other) {
}

TextureClass::~TextureClass() {
}

bool TextureClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* fileName) {	
	int width, height;
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT result;
	unsigned int rowPitch;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

	if (!LoadTarga(fileName, height, width)) {
		return false;
	}

	textureDesc.Height = height;
	textureDesc.Width = width;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	result = device->CreateTexture2D(&textureDesc, NULL, &m_texture2D);
	if (FAILED(result)) {
		return false;
	}

	rowPitch = (width * 4) * sizeof(unsigned char);

	//actually copying
	// use cache
	deviceContext->UpdateSubresource(m_texture2D, 0, NULL, m_targaData, rowPitch, 0);

	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	result = device->CreateShaderResourceView(m_texture2D, &srvDesc, &m_textureView);
	
	if (FAILED(result)) {
		return false;
	}

	deviceContext->GenerateMips(m_textureView);

	delete [] m_targaData;
	m_targaData = 0;

	return true;
}

void TextureClass::Shutdown() {
	if (m_textureView) {
		m_textureView->Release();
		m_textureView = 0;
	}
	if (m_texture2D) {
		m_texture2D->Release();
		m_texture2D = 0;
	}
	if (m_targaData) {
		delete [] m_targaData;
		m_targaData = 0;
	}

	return;
}

ID3D11ShaderResourceView* TextureClass::GetTexture() {
	return m_textureView;
}

bool TextureClass::LoadTarga(char* fileName , int& height, int& width) {
	int error, bpp, imageSize, index, k, i, j;
	FILE* filePtr;
	unsigned int count;
	TargaHeader targaFileHeader;
	unsigned char* targaImage;

	error = fopen_s(&filePtr, fileName, "rb");
	if (error != 0) {
		return false;
	}

	count = (unsigned int)fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr);
	if (count != 1) {
		return false;
	}

	height = (int)targaFileHeader.height;
	width = (int)targaFileHeader.width;
	bpp = (int)targaFileHeader.bpp;

	if (bpp != 32) {
		return false;
	}

	imageSize = width * height * 4;

	targaImage = new unsigned char[imageSize];
	if (!targaImage) {
		return false;
	}

	count = (unsigned int)fread(targaImage, 1, imageSize, filePtr);
	if (count!=imageSize) {
		return false;
	}

	error = fclose(filePtr);
	if (error != 0) {
		return false;
	}

	m_targaData = new unsigned char[imageSize];
	if (!m_targaData) {
		return false;
	}

	index = 0;

	k = (width * height * 4) - (width * 4);

	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			m_targaData[index + 0] = targaImage[k + 2];
			m_targaData[index + 1] = targaImage[k + 1];
			m_targaData[index + 2] = targaImage[k + 0];
			m_targaData[index + 3] = targaImage[k + 3];

			k += 4;
			index += 4;
		}
		k -= (width * 8);
	}

	delete [] targaImage;
	targaImage = 0;

	return true;
}