#include "stdafx.h"
#include "ProjectileManager.h"

#define INITIAL_POOL_COUNT 10

ProjectileManager::ProjectileManager()
{
	// TODO: should be passed in from Projectile JSON
	m_fLifeTime = 1.0f; 
	float fSpeed = 50.0f; 

	m_vecProjectilePool = std::vector<std::shared_ptr<Projectile>>();
	for (int i = 0; i < INITIAL_POOL_COUNT; i++)
		m_vecProjectilePool.push_back(std::make_shared<Projectile>(fSpeed));
	
	AddToEvent();
}

void ProjectileManager::Initialize(const Graphics& gfx, ConstantBuffer<Matrices>& mat)
{
	for(std::shared_ptr<Projectile>& pProjectile : m_vecProjectilePool)
		pProjectile->Initialize(gfx, mat, Sprite::Type::Projectile);
}

void ProjectileManager::InitialiseFromFile(const Graphics& gfx, ConstantBuffer<Matrices>& mat, const std::string& filename)
{
	// TODO: Load file into JSON
	
	// TODO: Find all projectile objects from JSON
	// TODO: Create projectile objects from JSON loaded data and Store in array
	for (std::shared_ptr<Projectile>& pProjectile : m_vecProjectilePool)
		pProjectile->Initialize(gfx, mat, filename);
}

void ProjectileManager::Update( const float dt )
{
	for (std::shared_ptr<Projectile> pProjectile : m_vecProjectilePool)
		pProjectile->Update(dt);
}

void ProjectileManager::Draw( ID3D11DeviceContext* context, XMMATRIX orthoMatrix )
{
	for (std::shared_ptr<Projectile> pProjectile : m_vecProjectilePool)
		pProjectile->Draw(context, orthoMatrix);
}

void ProjectileManager::ResetPool(ProjectileData::ProjectileJSON projectileData, const Graphics& gfx, ConstantBuffer<Matrices>& mat)
{
	m_vecProjectilePool.clear();
	InitialiseFromFile(gfx, mat, projectileData.m_sTexture);

	for (std::shared_ptr<Projectile>& pProjectile : m_vecProjectilePool)
	{
		pProjectile->SetSpeed(projectileData.m_fSpeed);
		pProjectile->SetLifeTime(projectileData.m_fLifeTime);
		pProjectile->SetDirection(Vector2f(projectileData.m_fAngle));
	}
}

void ProjectileManager::SpawnProjectile()
{
	std::shared_ptr<Projectile> pProjectile = GetFreeProjectile();

	if (pProjectile != nullptr)
		pProjectile->SpawnProjectile(m_vSpawnPosition, m_vTargetPosition, m_fLifeTime);
}

void ProjectileManager::SpawnProjectile(Vector2f vSpawnPosition, float fLifeTime)
{
	std::shared_ptr<Projectile> pProjectile = GetFreeProjectile();
	
	if(pProjectile != nullptr)
		pProjectile->SpawnProjectile(vSpawnPosition, m_fLifeTime);
}

std::shared_ptr<Projectile> ProjectileManager::GetFreeProjectile()
{
	for (std::shared_ptr<Projectile> pProjectile : m_vecProjectilePool)
		if (!pProjectile->IsAlive())
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