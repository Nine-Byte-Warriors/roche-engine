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
	EventSystem::Instance()->AddClient(EVENTID::LeftMouseClick, this);
	EventSystem::Instance()->AddClient(EVENTID::LeftMouseRelease, this);
	EventSystem::Instance()->AddClient(EVENTID::MouseCameraPosition, this);
}

void PlayerShooting::RemoveFromEvent() noexcept
{
	EventSystem::Instance()->RemoveClient(EVENTID::PlayerFire, this);
	EventSystem::Instance()->RemoveClient(EVENTID::LeftMouseClick, this);
	EventSystem::Instance()->RemoveClient(EVENTID::LeftMouseRelease, this);
	EventSystem::Instance()->RemoveClient(EVENTID::MouseCameraPosition, this);
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
	case EVENTID::MouseCameraPosition:
	{
		m_vMousePos = *static_cast<Vector2f*>(event->GetData());
		m_pEmitter->SetTargetPosition(m_vMousePos);

		break;
	}
	case EVENTID::LeftMouseClick:
	{
		m_pEmitter->SetTargetPosition(m_vMousePos);
		m_bIsShooting = true;
		break;
	}
	case EVENTID::LeftMouseRelease:
	{
		m_pEmitter->SetTargetPosition(m_vMousePos);
		m_bIsShooting = false;
		break;
	}
	}
}
