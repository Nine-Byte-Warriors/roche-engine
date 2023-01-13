#include "stdafx.h"
#include "ProjectileManager.h"

ProjectileManager::ProjectileManager()
{
	m_fLifeTime = 2.50f; // TODO: should be passed in from Projectile JSON

	m_projectile = std::make_shared<Projectile>(10.0f);
	
	AddToEvent();
}

void ProjectileManager::Initialize(const Graphics& gfx, ConstantBuffer<Matrices>& mat)
{
	m_projectile->Initialize(gfx, mat, Sprite::Type::Projectile);
}

void ProjectileManager::Update( const float dt )
{
	m_projectile->Update( dt );
}

void ProjectileManager::Draw( ID3D11DeviceContext* context, XMMATRIX orthoMatrix )
{
	m_projectile->Draw(context, orthoMatrix);
}

void ProjectileManager::SpawnProjectile()
{
	m_projectile->SpawnProjectile(m_vSpawnPosition, m_vTargetPosition, m_fLifeTime);
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
		SpawnProjectile();
		break;
	default:
		break;
	}
}