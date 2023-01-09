#include "stdafx.h"
#include "Camera2D.h"
#include "EventSystem.h"

Camera2D::Camera2D()
{
	m_transform = std::make_shared<Transform>();
	m_transform->SetPosition( XMFLOAT3( 0.0f, 0.0f, 0.0f ) );
	m_transform->SetRotation( XMFLOAT3( 0.0f, 0.0f, 0.0f ) );
	UpdateMatrix();
}

void Camera2D::SetProjectionValues( float width, float height, float nearZ, float farZ )
{
	orthoMatrix = XMMatrixOrthographicOffCenterLH( 0.0f, width, height, 0.0f, nearZ, farZ );
}

const XMMATRIX& Camera2D::GetOrthoMatrix() const noexcept
{
	return orthoMatrix;
}

const XMMATRIX& Camera2D::GetWorldOrthoMatrix() const noexcept
{
	return m_transform->GetWorldMatrix() * orthoMatrix;
}

void Camera2D::SendWorldOrthoMatrix()
{
	// WorldOrthoMatrix Output
	XMStoreFloat4x4( &WorldOrthoMatrix, m_transform->GetWorldMatrix() * orthoMatrix );
	EventSystem::Instance()->AddEvent( EVENTID::WorldOrthMatrixEvent, &WorldOrthoMatrix );
}

void Camera2D::UpdateMatrix()
{
	XMMATRIX translationOffsetMatrix = XMMatrixTranslation(
		-m_transform->GetPositionFloat3().x,
		-m_transform->GetPositionFloat3().y, 0.0f );
	XMMATRIX cameraRotationMatrix = XMMatrixRotationRollPitchYaw(
		m_transform->GetRotationFloat3().x,
		m_transform->GetRotationFloat3().y,
		m_transform->GetRotationFloat3().z );
	m_transform->SetWorldMatrix( cameraRotationMatrix * translationOffsetMatrix );
}