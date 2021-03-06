#ifndef CAMERACLASS_H_
#define CAMERACLASS_H_

#include <DirectXMath.h>

class CameraClass {
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float,float,float);
	void SetRotation(float, float, float);

	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetRotation();

	void Render();
	void GetViewMatrix(DirectX::XMMATRIX&);

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	DirectX::XMMATRIX m_viewMatrix;
};

#endif // !CAMERACLASS_H_