#include "stdafx.h"
#include "Camera.h"
#include "Sprite.h"
#include "EventSystem.h"

#if _DEBUG
#include <imgui/imgui.h>
#endif

void Camera::SetProjectionValues( float width, float height, float nearZ, float farZ )
{
	m_vSizeOfScreen = { width, height };
	m_vPosition = { m_vSizeOfScreen.x / 2.0f, m_vSizeOfScreen.y / 2.0f };
	m_mOrthoMatrix = XMMatrixOrthographicOffCenterLH( 0.0f, width, height, 0.0f, nearZ, farZ );

	m_vInitPosition = m_vPosition;
}

void Camera::SpawnControlWindow()
{
#if _DEBUG
	if ( ImGui::Begin( "Camera Editor", FALSE, ImGuiWindowFlags_AlwaysAutoResize ) )
	{
		ImGui::Text( "Move Speed" );
		ImGui::SliderFloat( "##Move Speed", &m_fSpeed, 1.0f, 20.0f, "%.1f" );
		ImGui::NewLine();
		ImGui::Checkbox( "Lock To Player?", &m_bLockedToPlayer );
	}
	ImGui::End();
#endif
}

void Camera::Update( const float dt )
{
	if ( !m_bLockedToPlayer )
	{
		m_mWorldMatrix = XMMatrixTranslation(
			-( m_vPosition.x - m_vSizeOfScreen.x / 2.0f ),
			-( m_vPosition.y - m_vSizeOfScreen.y / 2.0f ), 0.0f );
	}
}

void Camera::AddToEvent() noexcept
{
	EventSystem::Instance()->AddClient( EVENTID::WindowSizeChangeEvent, this );
	EventSystem::Instance()->AddClient( EVENTID::CameraUp, this );
	EventSystem::Instance()->AddClient( EVENTID::CameraLeft, this );
	EventSystem::Instance()->AddClient( EVENTID::CameraDown, this );
	EventSystem::Instance()->AddClient( EVENTID::CameraRight, this );
	EventSystem::Instance()->AddClient( EVENTID::LockCameraToPlayer, this );
}

void Camera::RemoveFromEvent() noexcept
{
	EventSystem::Instance()->RemoveClient( EVENTID::WindowSizeChangeEvent, this );
	EventSystem::Instance()->RemoveClient( EVENTID::CameraUp, this );
	EventSystem::Instance()->RemoveClient( EVENTID::CameraLeft, this );
	EventSystem::Instance()->RemoveClient( EVENTID::CameraDown, this );
	EventSystem::Instance()->RemoveClient( EVENTID::CameraRight, this );
	EventSystem::Instance()->RemoveClient( EVENTID::LockCameraToPlayer, this );
}

void Camera::HandleEvent( Event* event )
{
	switch ( event->GetEventID() )
	{
	case EVENTID::WindowSizeChangeEvent:
	{
		m_vSizeOfScreen = *static_cast<XMFLOAT2*>( event->GetData() );
		if ( m_vSizeOfScreen.x < 500.0f ) m_vSizeOfScreen.x = 1260.0f;
		if ( m_vSizeOfScreen.y < 400.0f ) m_vSizeOfScreen.y = 500.0f;
		SetProjectionValues( m_vSizeOfScreen.x, m_vSizeOfScreen.y, 0.0f, 1.0f );
	}
	break;
	case EVENTID::LockCameraToPlayer:
	{
		if ( m_bLockedToPlayer )
		{
			std::pair<Sprite*, Vector2f*>* charSpriteandPos = static_cast<std::pair<Sprite*, Vector2f*>*>(event->GetData());
			Sprite* charSprite = charSpriteandPos->first;
			m_vPosition = XMFLOAT2(charSpriteandPos->second->x + charSprite->GetWidth(), charSpriteandPos->second->y + charSprite->GetHeight());

			m_mWorldMatrix = XMMatrixTranslation(
				-(m_vPosition.x - m_vSizeOfScreen.x / 2.0f),
				-(m_vPosition.y - m_vSizeOfScreen.y / 2.0f), 0.0f);
		}
	}
	break;
	case EVENTID::CameraUp: if ( !m_bLockedToPlayer ) { m_vPosition.y -= m_fSpeed; } break;
	case EVENTID::CameraDown: if ( !m_bLockedToPlayer ) { m_vPosition.y += m_fSpeed; } break;
	case EVENTID::CameraRight: if ( !m_bLockedToPlayer ) { m_vPosition.x += m_fSpeed; } break;
	case EVENTID::CameraLeft: if ( !m_bLockedToPlayer ) { m_vPosition.x -= m_fSpeed; } break;
	}
}