#include "stdafx.h"
#include "PlayerShooting.h"

#if _DEBUG
extern bool g_bDebug;
#endif

void PlayerShooting::Update(float dt)
{
	// handle mouse shooting actions/event
	if (m_bIsShooting)
	{
		m_pEmitter->EmitProjectile();

		// TODO : alter disabling of shooting dependent on the type of shooting behaviour required
		m_bIsShooting = false;
	}
}

void PlayerShooting::AddToEvent() noexcept
{
	EventSystem::Instance()->AddClient(EVENTID::PlayerFire, this);
	EventSystem::Instance()->AddClient(EVENTID::ImGuiMousePosition, this);
	EventSystem::Instance()->AddClient(EVENTID::MousePosition, this);
	EventSystem::Instance()->AddClient(EVENTID::LeftMouseClick, this);
	EventSystem::Instance()->AddClient(EVENTID::LeftMouseRelease, this);
}

void PlayerShooting::RemoveFromEvent() noexcept
{
	EventSystem::Instance()->RemoveClient(EVENTID::PlayerFire, this);
	EventSystem::Instance()->RemoveClient(EVENTID::ImGuiMousePosition, this);
	EventSystem::Instance()->RemoveClient(EVENTID::MousePosition, this);
	EventSystem::Instance()->RemoveClient(EVENTID::LeftMouseClick, this);
	EventSystem::Instance()->RemoveClient(EVENTID::LeftMouseRelease, this);
}

void PlayerShooting::HandleEvent(Event* event)
{
	switch (event->GetEventID())
	{
		case EVENTID::PlayerFire:
		{
			m_bIsShooting = true;
			break;
		}
#if _DEBUG
		case EVENTID::ImGuiMousePosition:
		{
			if ( !g_bDebug ) return;
			m_vMousePos = *(Vector2f*)event->GetData();
			m_pEmitter->SetTargetPosition( m_vMousePos );
			break;
		}
#endif
		case EVENTID::MousePosition:
		{
#if _DEBUG
			if ( g_bDebug ) return;
#endif
			m_vMousePos = *(Vector2f*)event->GetData();
			m_pEmitter->SetTargetPosition( *(Vector2f*)event->GetData() );
			break;
		}
		case EVENTID::LeftMouseClick:
		{
			m_pEmitter->SetTargetPosition( m_vMousePos );
			m_bIsShooting = true;
			break;
		}
		case EVENTID::LeftMouseRelease:
		{
			m_pEmitter->SetTargetPosition( m_vMousePos );
			m_bIsShooting = false;
			break;
		}
	}
}
