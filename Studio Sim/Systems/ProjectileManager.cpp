#include "stdafx.h"
#include "ProjectileManager.h"

#define INITIAL_POOL_COUNT 10

ProjectileManager::ProjectileManager()
{
	m_fLifeTime = 2.50f; // TODO: should be passed in from Projectile JSON

	m_projectile = std::make_shared<Projectile>(10.0f);
	m_projectiles = std::vector<std::shared_ptr<Projectile>>();
	
	for (int i = 0; i < INITIAL_POOL_COUNT; i++)
		m_projectiles.push_back(std::make_shared<Projectile>(10.0f));
	
	AddToEvent();
}

void ProjectileManager::Initialize(const Graphics& gfx, ConstantBuffer<Matrices>& mat)
{
	m_projectile->Initialize(gfx, mat, Sprite::Type::Projectile);
	
	for(std::shared_ptr<Projectile>& pProjectile : m_projectiles)
		pProjectile->Initialize(gfx, mat, Sprite::Type::Projectile);
}

void ProjectileManager::Update( const float dt )
{
	m_projectile->Update( dt );
	
	for (std::shared_ptr<Projectile>& pProjectile : m_projectiles)
		pProjectile->Update(dt);
}

void ProjectileManager::Draw( ID3D11DeviceContext* context, XMMATRIX orthoMatrix )
{
	m_projectile->Draw(context, orthoMatrix);

	for (std::shared_ptr<Projectile>& pProjectile : m_projectiles)
		pProjectile->Draw(context, orthoMatrix);
}

void ProjectileManager::SpawnProjectile()
{
	m_projectile->SpawnProjectile(m_vSpawnPosition, m_vTargetPosition, m_fLifeTime);
	
	std::shared_ptr<Projectile> pProjectile = GetFreeProjectile();
	
	if(pProjectile != nullptr)
		pProjectile->SpawnProjectile(m_vSpawnPosition, m_vTargetPosition, m_fLifeTime);
}

std::shared_ptr<Projectile> ProjectileManager::GetFreeProjectile()
{
	for (std::shared_ptr<Projectile>& pProjectile : m_projectiles)
		if (pProjectile->IsAlive())
			return pProjectile;
	
	return nullptr;
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