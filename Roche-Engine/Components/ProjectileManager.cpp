#include "stdafx.h"
#include "ProjectileManager.h"

#define INITIAL_POOL_COUNT 10

ProjectileManager::ProjectileManager()
{
	// TODO: should be passed in from Projectile JSON
	m_fLifeTime = 1.0f;
	float fSpeed = 50.0f;
	m_fDelay = 0.0f;
	m_fCounter = 0.0f;

	m_vecProjectilePayLoads = std::vector<std::shared_ptr<ProjectilePayLoad>>();
	
	AddToEvent();
}

ProjectileManager::~ProjectileManager()
{
	for (std::shared_ptr<ProjectilePayLoad> pProjectilePayLoad : m_vecProjectilePayLoads)
		pProjectilePayLoad->m_vecProjectilePool.clear();
	
	RemoveFromEvent();
}

void ProjectileManager::Initialize(const Graphics& gfx, ConstantBuffer<Matrices>& mat)
{
	for (std::shared_ptr<ProjectilePayLoad> vecPayLoad : m_vecProjectilePayLoads)
	{
		for (std::shared_ptr<Projectile>& pProjectile : vecPayLoad->m_vecProjectilePool)
			pProjectile->Initialize(gfx, mat, Sprite::Type::Projectile);
	}
}

void ProjectileManager::InitialiseFromFile(const Graphics& gfx, ConstantBuffer<Matrices>& mat, const std::string& filename)
{
	for (std::shared_ptr<ProjectilePayLoad> pProjectilePayLoad : m_vecProjectilePayLoads)
	{
		if (!pProjectilePayLoad->IsActive())
			continue;

		for (std::shared_ptr<Projectile> pProjectile : pProjectilePayLoad->m_vecProjectilePool)
			pProjectile->Initialize(gfx, mat, filename);
	}
}

void ProjectileManager::Update( const float dt )
{
	if (m_fCounter >= 0.0f)
	{
		m_fCounter -= dt;
		return;
	}

	for (std::shared_ptr<ProjectilePayLoad> pProjectilePayLoad : m_vecProjectilePayLoads)
	{
		if (!pProjectilePayLoad->IsActive())
			continue;

		for (std::shared_ptr<Projectile> pProjectile : pProjectilePayLoad->m_vecProjectilePool)
			pProjectile->Update(dt);
	}
}

void ProjectileManager::Draw( ID3D11DeviceContext* context, XMMATRIX orthoMatrix )
{
	for (std::shared_ptr<ProjectilePayLoad> pProjectilePayLoad : m_vecProjectilePayLoads)
	{
		if (!pProjectilePayLoad->IsActive())
			continue;
		
		for (std::shared_ptr<Projectile> pProjectile : pProjectilePayLoad->m_vecProjectilePool)
			pProjectile->Draw(context, orthoMatrix);
	}
}

void ProjectileManager::SpawnProjectile()
{
	m_fCounter = m_fDelay;

	std::shared_ptr<Projectile> pProjectile = GetFreeProjectile();

	if (pProjectile != nullptr)
		pProjectile->SpawnProjectile(m_vSpawnPosition, m_vTargetPosition, m_fLifeTime);
}

void ProjectileManager::UpdatePattern(std::string filepath)
{
	JsonLoading::LoadJson(m_vecManagers, filepath);

	for (int i = 0; i < m_vecManagers.size(); i++)
	{
		m_fDelay = m_vecManagers[i].m_fDelay;
		SetPayLoadPool(m_vecManagers[i].m_vecProjectiles);
	}
}

std::vector<std::shared_ptr<Projectile>> ProjectileManager::CreateProjectilePool(std::vector<ProjectileData::ProjectileJSON> vecProjectileJsons)
{
	std::vector<std::shared_ptr<Projectile>> vecProjectilePool;

	for (ProjectileData::ProjectileJSON pJson : vecProjectileJsons)
	{
		std::shared_ptr<Projectile> pProjectile = std::make_shared<Projectile>(pJson.m_fSpeed, pJson.m_fLifeTime);
		pProjectile->SetDirection(Vector2f(pJson.m_fAngle));
		pProjectile->SetOffSet(Vector2f(pJson.m_fX, pJson.m_fY));
		pProjectile->SetWave(pJson.m_fAngle, pJson.m_fAmplitude, pJson.m_fFrequency);

		vecProjectilePool.push_back(std::move(pProjectile));
	}

	return vecProjectilePool;
}

void  ProjectileManager::SetPayLoadPool(std::vector<ProjectileData::ProjectileJSON> vecProjectileJsons)
{
	for (std::shared_ptr<ProjectilePayLoad> pPayLoad : m_vecProjectilePayLoads)
		pPayLoad->m_vecProjectilePool.clear();

	m_vecProjectilePayLoads.clear();

	for (int i = 0; i < INITIAL_POOL_COUNT; i++)
	{
		std::unique_ptr< ProjectilePayLoad> pPayLoad = std::make_unique<ProjectilePayLoad>();
		pPayLoad->m_bActive = false;
		pPayLoad->m_vecProjectilePool = CreateProjectilePool(vecProjectileJsons);
		m_vecProjectilePayLoads.push_back(std::move(pPayLoad));
	}
}

void ProjectileManager::SpawnProjectile(Vector2f vSpawnPosition, float fLifeTime)
{
	m_fCounter = m_fDelay;

	std::shared_ptr<Projectile> pProjectile = GetFreeProjectile();

	if(pProjectile != nullptr)
		pProjectile->SpawnProjectile(vSpawnPosition, m_fLifeTime);
}

void ProjectileManager::SpawnProjectiles(Vector2f vSpawnPosition)
{
	m_fCounter = m_fDelay;

	std::shared_ptr<ProjectilePayLoad> pProjectilePayLoads = GetProjectilePayLoad();
	
	if (pProjectilePayLoads == nullptr)
		return;
	
	for (std::shared_ptr<Projectile> pProjectile : pProjectilePayLoads->m_vecProjectilePool)
		pProjectile->SpawnProjectile(vSpawnPosition, -1.0f);
}

std::shared_ptr<Projectile> ProjectileManager::GetFreeProjectile()
{
	// TODO : Remove calls to this method and replace with calls to GetProjectilePayLoad().
	
	//for (std::pair<std::shared_ptr<Projectile>, bool> vecPayLoad : m_mapProjectilePool)
	//	for (std::shared_ptr<Projectile> pProjectile : vecPayLoad)
	//		if (!pProjectile->IsAlive())
	//			return pProjectile;
	
	/*for (std::shared_ptr<Projectile> pProjectile : m_vecProjectilePool)
		if (!pProjectile->IsAlive())
			return pProjectile;*/

	return nullptr;
}

std::shared_ptr<ProjectilePayLoad> ProjectileManager::GetProjectilePayLoad()
{
	for (std::shared_ptr<ProjectilePayLoad> pProjectilePayLoad : m_vecProjectilePayLoads)
		if (!pProjectilePayLoad->IsActive())
			return pProjectilePayLoad;

	return nullptr;
}

void ProjectileManager::SetProjectileScaleInit(const float fScaleX, const float fScaleY)
{
	for (std::shared_ptr<ProjectilePayLoad> pProjectilePayLoad : m_vecProjectilePayLoads)
		for (std::shared_ptr<Projectile> pProjectile : pProjectilePayLoad->m_vecProjectilePool)
			pProjectile->GetTransform()->SetScaleInit(fScaleX, fScaleY);
}

void ProjectileManager::SetProjectileMaxFrame(const float fFrameX, const float fFrameY)
{
	for (std::shared_ptr<ProjectilePayLoad> pProjectilePayLoad : m_vecProjectilePayLoads)
		for (std::shared_ptr<Projectile> pProjectile : pProjectilePayLoad->m_vecProjectilePool)
			pProjectile->GetSprite()->SetMaxFrame(fFrameX, fFrameY);
}

void ProjectileManager::UpdateProjectileTexture(ID3D11Device* pDevice, std::string sTexture)
{
	for (std::shared_ptr<ProjectilePayLoad> pProjectilePayLoad : m_vecProjectilePayLoads)
		for (std::shared_ptr<Projectile> pProjectile : pProjectilePayLoad->m_vecProjectilePool)
			pProjectile->GetSprite()->UpdateTex(pDevice, sTexture);
}

void ProjectileManager::UpdateProjectileMass(const float fMass)
{
	for (std::shared_ptr<ProjectilePayLoad> pProjectilePayLoad : m_vecProjectilePayLoads)
		for (std::shared_ptr<Projectile> pProjectile : pProjectilePayLoad->m_vecProjectilePool)
			pProjectile->GetPhysics()->SetMass(fMass);
}

void ProjectileManager::AddToEvent() noexcept
{
	EventSystem::Instance()->AddClient(EVENTID::PlayerPosition, this);
	EventSystem::Instance()->AddClient(EVENTID::TargetPosition, this);
	EventSystem::Instance()->AddClient(EVENTID::PlayerFire, this);
}

void ProjectileManager::RemoveFromEvent() noexcept
{
	EventSystem::Instance()->RemoveClient(EVENTID::PlayerPosition, this);
	EventSystem::Instance()->RemoveClient(EVENTID::TargetPosition, this);
	EventSystem::Instance()->RemoveClient(EVENTID::PlayerFire, this);
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