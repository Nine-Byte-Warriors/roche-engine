#include "stdafx.h"
#include "PlayerShooting.h"

void PlayerShooting::Update(float dt)
{
	// handle mouse shooting actions/event
	if (m_bIsShooting)
	{
		m_pEmitter->EmitProjectile();

		// TODO : alter disabling of shooting dependant on the type oshooting behaviour required.
		m_bIsShooting = false;
	}
}

void PlayerShooting::AddToEvent() noexcept
{
	EventSystem::Instance()->AddClient(EVENTID::PlayerFire, this);
	EventSystem::Instance()->AddClient(EVENTID::MousePosition, this);
	EventSystem::Instance()->AddClient(EVENTID::LeftMouseClick, this);
	EventSystem::Instance()->AddClient(EVENTID::LeftMouseRelease, this);
}

void PlayerShooting::RemoveFromEvent() noexcept
{
	EventSystem::Instance()->RemoveClient(EVENTID::PlayerFire, this);
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
		case EVENTID::MousePosition:
		{
			m_pEmitter->SetTargetPosition(*static_cast<Vector2f*>(event->GetData()));
			break;
		}
		case EVENTID::LeftMouseClick:
		{
			m_pEmitter->SetTargetPosition(*static_cast<Vector2f*>(event->GetData()));
			m_bIsShooting = true;
			break;
		}
		case EVENTID::LeftMouseRelease:
		{
			m_pEmitter->SetTargetPosition(*static_cast<Vector2f*>(event->GetData()));
			m_bIsShooting = false;
			break;
		}
	}
}
