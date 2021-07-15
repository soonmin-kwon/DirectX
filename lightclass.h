#ifndef _LIGHTCLASS_H_
#define _LIGHTCLASS_H_

#include <DirectXMath.h>

class LightClass {
public:
	LightClass();
	LightClass(const LightClass&);
	~LightClass();

	void SetSpecularColor(float, float, float, float);
	void SetDiffuseColor(float, float, float, float);
	void SetAmbientColor(float, float, float, float);	
	void SetDirection(float, float, float);
	void SetSpecularPower(float);
	
	DirectX::XMFLOAT4 GetSpecularColor();
	DirectX::XMFLOAT4 GetDiffuseColor();
	DirectX::XMFLOAT4 GetAmbientColor();	
	DirectX::XMFLOAT3 GetDirection();	
	float GetSpecularPower();
	
private:	
	DirectX::XMFLOAT4 m_specularColor;
	DirectX::XMFLOAT4 m_diffuseColor;
	DirectX::XMFLOAT4 m_ambientColor;
	DirectX::XMFLOAT3 m_direction;
	float m_specularPower;
};

#endif // !_LIGHTCLASS_H_