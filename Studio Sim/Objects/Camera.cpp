#include "stdafx.h"
#include "Camera.h"
#include "EventSystem.h"

Camera::Camera()
{
	m_transform = std::make_shared<Transform>();
	UpdateMatrix();
}

void Camera::SetProjectionValues( float width, float height, float nearZ, float farZ )
{
	orthoMatrix = XMMatrixOrthographicOffCenterLH( 0.0f, width, height, 0.0f, nearZ, farZ );
}

const XMMATRIX& Camera::GetOrthoMatrix() const noexcept
{
	return orthoMatrix;
}

const XMMATRIX& Camera::GetWorldOrthoMatrix() const noexcept
{
	return m_transform->GetWorldMatrix() * orthoMatrix;
}

void Camera::UpdateMatrix()
{
	XMMATRIX translationOffsetMatrix = XMMatrixTranslation(
		-m_transform->GetPosition().x,
		-m_transform->GetPosition().y, 0.0f );
	XMMATRIX cameraRotationMatrix = XMMatrixRotationRollPitchYaw(
		0.0f, 0.0f,
		m_transform->GetRotation() );
}