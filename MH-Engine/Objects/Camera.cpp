#include "stdafx.h"
#include "Camera.h"
#include "EventSystem.h"

Camera::Camera()
{
	m_transform = std::make_shared<Transform>();
	UpdateMatrix();
	AddToEvent();
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

void Camera::AddToEvent() noexcept
{
	EventSystem::Instance()->AddClient( EVENTID::WindowSizeChangeEvent, this );
}

void Camera::HandleEvent( Event* event )
{
	switch ( event->GetEventID() )
	{
	case EVENTID::WindowSizeChangeEvent:
	{
		XMFLOAT2 sizeOfScreen = *static_cast<XMFLOAT2*>( event->GetData() );

		if ( sizeOfScreen.x < 500 )
			sizeOfScreen.x = 1260;

		if ( sizeOfScreen.y < 400 )
			sizeOfScreen.y = 500;

		SetProjectionValues( sizeOfScreen.x, sizeOfScreen.y, 0.0f, 1.0f );
	}
	break;
	}
}