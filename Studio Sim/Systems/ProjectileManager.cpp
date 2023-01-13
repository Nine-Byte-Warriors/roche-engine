#include "stdafx.h"
#include "ProjectileManager.h"

ProjectileManager::ProjectileManager()
{
	m_fLifeTime = 5.0f;	// TODO: should be passed in from Projectile JSON
	m_pProjectile = std::make_shared<Projectile>();

	AddToEvent();
}

void ProjectileManager::Initialize(const Graphics& gfx, ConstantBuffer<Matrices>& mat)
{
	m_pProjectile->Initialize(gfx, mat, Sprite::Type::Projectile);
	m_pProjectile->GetTransform()->SetPositionInit(m_pProjectile->GetTransform()->GetPosition().x, m_pProjectile->GetTransform()->GetPosition().y);
	m_pProjectile->GetTransform()->SetScaleInit(m_pProjectile->GetSprite()->GetWidth(), m_pProjectile->GetSprite()->GetHeight());
}

void ProjectileManager::AddToEvent() noexcept
{
	EventSystem::Instance()->AddClient(EVENTID::PlayerPosition, this);
	EventSystem::Instance()->AddClient(EVENTID::TargetPosition, this);
	EventSystem::Instance()->AddClient(EVENTID::PlayerFire, this);
}

void ProjectileManager::HandleEvent(Event* event)
{
	switch (event->GetEventID())
	{
	case EVENTID::PlayerPosition: 
		m_vSpawnPosition = *static_cast<Vector2f*>(event->GetData());
		break;
	case EVENTID::TargetPosition: 
		m_vTargetPosition = *static_cast<Vector2f*>(event->GetData());
		break;
	case EVENTID::PlayerFire:
		m_pProjectile->SpawnProjectile(m_vSpawnPosition, m_vTargetPosition, m_fLifeTime);
		break;
	default:
		break;
	}
}