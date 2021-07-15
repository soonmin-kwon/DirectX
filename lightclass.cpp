#include "lightclass.h"
using namespace DirectX;

LightClass::LightClass() {
}

LightClass::LightClass(const LightClass& other) {
}

LightClass::~LightClass() {
}

void LightClass::SetSpecularColor(float r, float g, float b, float a) {
	m_specularColor = XMFLOAT4(r, g, b, a);

	return;
}

void LightClass::SetDiffuseColor(float r, float g, float b, float a) {
	m_diffuseColor = XMFLOAT4(r, g, b, a);

	return;
}

void LightClass::SetAmbientColor(float r, float g, float b, float a) {
	m_ambientColor = XMFLOAT4(r, g, b, a);

	return;
}

void LightClass::SetDirection(float x, float y, float z) {
	m_direction = XMFLOAT3(x, y, z);

	return;
}

void LightClass::SetSpecularPower(float power) {
	m_specularPower = power;

	return;
}

XMFLOAT4 LightClass::GetSpecularColor() {
	return m_specularColor;
}

XMFLOAT4 LightClass::GetDiffuseColor() {
	return m_diffuseColor;
}

XMFLOAT4 LightClass::GetAmbientColor() {
	return m_ambientColor;
}

XMFLOAT3 LightClass::GetDirection() {
	return m_direction;
}

float LightClass::GetSpecularPower() {
	return m_specularPower;
}