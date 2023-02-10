#include "stdafx.h"
#include "PlayerShooting.h"
#include "AudioEngine.h"

#define PLAYER "Player"

#if _DEBUG
extern bool g_bDebug;
#endif

void PlayerShooting::Update(float dt)
{
	static float shootingDelay = *m_fShootingDelay;
	static bool firstNight = true;
	if (m_currentGamePhase == Phase::NightPhase)
	{
		if (firstNight)
		{
			shootingDelay = *m_fShootingDelay;
			if (shootingDelay > 0)
			{
				firstNight = false;
			}
		}
		shootingDelay -= dt;
	}

	// handle mouse shooting actions/event
	if (m_bIsShooting && shootingDelay < 0 /*&& !firstNight*/)
	{
		m_pEmitter->EmitProjectile();
		AudioEngine::GetInstance()->PlayAudio(PLAYER, "EntityBulletShot", SFX);

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
	EventSystem::Instance()->AddClient(EVENTID::CurrentPhase, this);
	EventSystem::Instance()->AddClient(EVENTID::ShootingDelay, this);
}

void PlayerShooting::RemoveFromEvent() noexcept
{
	EventSystem::Instance()->RemoveClient(EVENTID::PlayerFire, this);
	EventSystem::Instance()->RemoveClient(EVENTID::LeftMouseClick, this);
	EventSystem::Instance()->RemoveClient(EVENTID::LeftMouseRelease, this);
	EventSystem::Instance()->RemoveClient(EVENTID::MouseCameraPosition, this);
	EventSystem::Instance()->RemoveClient(EVENTID::CurrentPhase, this);
	EventSystem::Instance()->RemoveClient(EVENTID::ShootingDelay, this);
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
		//m_bIsShooting = false;
		break;
	}
	case EVENTID::CurrentPhase:
	{
		m_currentGamePhase = *static_cast<Phase*>(event->GetData());
		break;
	}
	case EVENTID::ShootingDelay:
	{
		m_fShootingDelay = static_cast<float*>(event->GetData());
		break;
	}
	}
}
