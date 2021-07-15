#include "cameraclass.h"
using namespace DirectX;

CameraClass::CameraClass() {
	m_positionX = 0;
	m_positionY = 0;
	m_positionZ = 0;

	m_rotationX = 0;
	m_rotationY = 0;
	m_rotationZ = 0;
}

CameraClass::CameraClass(const CameraClass&) {
}

CameraClass::~CameraClass() {
}

void CameraClass::SetPosition(float x, float y, float z) {
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;

	return;
}

void CameraClass::SetRotation(float x, float y, float z) {
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
}

XMFLOAT3 CameraClass::GetPosition() {
	return XMFLOAT3(m_positionX, m_positionY, m_positionZ);
}

XMFLOAT3 CameraClass::GetRotation() {
	return XMFLOAT3(m_rotationX, m_rotationY, m_rotationZ);
}

void CameraClass::Render() {
	XMFLOAT3 up, position, lookAt;
	XMVECTOR upVector, posVector, lookAtVector;
	float pitch, yaw, roll;
	XMMATRIX rotationMatrix;

	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	upVector = XMLoadFloat3(&up);

	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	posVector = XMLoadFloat3(&position);

	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	lookAtVector = XMLoadFloat3(&lookAt);

	pitch = m_rotationX * 0.0174532925f;
	yaw = m_rotationY * 0.0174532925f;
	roll = m_rotationZ * 0.0174532925f;

	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	lookAtVector = XMVectorAdd(posVector, lookAtVector);

	m_viewMatrix = XMMatrixLookAtLH(posVector, lookAtVector, upVector);

	return;
}

void CameraClass::GetViewMatrix(XMMATRIX& viewMatrix) {
	viewMatrix = m_viewMatrix;

	return;
}