#ifndef _TEXTURECLASS_H_
#define _TEXTURECLASS_H_

#include <d3d11.h>
#include <stdio.h>

class TextureClass {
public:
	TextureClass();
	TextureClass(const TextureClass&);
	~TextureClass();
	
	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

private:
	struct TargaHeader {
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};

	bool LoadTarga(char*, int&, int&);

	// hold raw targa data
	unsigned char* m_targaData;
	// hold structured texture data
	ID3D11Texture2D* m_texture2D;
	// shader uses to access texture data
	ID3D11ShaderResourceView* m_textureView;
};

#endif // !TEXTURECLASS_H_