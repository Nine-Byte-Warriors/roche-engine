#include "stdafx.h"
#include "ProjectileManager.h"

ProjectileManager::ProjectileManager(Projectile::ProjectileOwner owner, std::string type)
{
	// TODO: should be passed in from Projectile JSON
	m_fLifeTime = 1.0f;
	float fSpeed = 50.0f;
	m_fDelay = 0.0f;
	m_fCounter = 0.0f;
	m_fTotalDuration = 0.0f;

	m_bWillRepeat = false;

	m_vecProjectilePool = std::vector<std::shared_ptr<Projectile>>();
	for (int i = 0; i < INITIAL_POOL_COUNT; i++)
		m_vecProjectilePool.push_back(std::make_shared<Projectile>(fSpeed, type));
	
	m_owner = owner;
	m_type = type;
}

ProjectileManager::~ProjectileManager()
{
	m_vecProjectilePool.clear();
}

std::vector<std::shared_ptr<Projectile>> ProjectileManager::CreateProjectilePool(std::vector<ProjectileData::ProjectileJSON> vecProjectileJsons, CollisionHandler* handler, std::string type ,float fGlobalSpeed, bool bUseGlobalSpeed)
{
	std::vector<std::shared_ptr<Projectile>> vecProjectilePool;

	for (ProjectileData::ProjectileJSON pJson : vecProjectileJsons)
	{
		std::shared_ptr<Projectile> pProjectile = std::make_shared<Projectile>(
			bUseGlobalSpeed == true ? fGlobalSpeed : pJson.m_fSpeed, type, pJson.m_fLifeTime
		);
		pProjectile->SetDirection(Vector2f(pJson.m_fAngle));
		pProjectile->SetOffSet(Vector2f(pJson.m_fX, pJson.m_fY));
		pProjectile->SetWave(pJson.m_fAngle, pJson.m_fAmplitude, pJson.m_fFrequency);
		pProjectile->SetDelay(pJson.m_fDelay);
		pProjectile->SetOwner(Projectile::ProjectileOwner::None);
		handler->AddCollider(pProjectile->GetCollider());
		

		vecProjectilePool.push_back(std::move(pProjectile));
	}

	return vecProjectilePool;

}

std::vector<std::shared_ptr<ProjectileManager>> ProjectileManager::GenerateManagers(std::string filepath, CollisionHandler* handler, std::string type)
{
	std::vector<std::shared_ptr<ProjectileManager>> vecManagers;
	std::vector<ProjectileData::ManagerJSON> vecManagersJson;
	JsonLoading::LoadJson(vecManagersJson, /*PATTERN_FOLDER_PATH + */ filepath);

	for (ProjectileData::ManagerJSON jMan : vecManagersJson)
	{
		std::shared_ptr <ProjectileManager> pManager = std::make_shared<ProjectileManager>(Projectile::ProjectileOwner::None, type);
		pManager->SetDelay(jMan.m_fDelay);

		std::vector<std::shared_ptr<ProjectilePayLoad>> vecPayLoads;
		for (int i = 0; i < INITIAL_POOL_COUNT; i++)
		{
			std::shared_ptr <ProjectilePayLoad> pPayload = std::make_shared<ProjectilePayLoad>(
				CreateProjectilePool(
					jMan.m_vecProjectiles,
					handler,
					type,
					jMan.m_fGlobalSpeed,
					jMan.m_bUseGlobalSpeed
				),
				jMan.m_sImagePath
			);
			pManager->m_vecPayLoads.push_back(pPayload);
			pManager->m_fTotalDuration = jMan.m_fDelay;
		}

		//pManager->SetProjectilePool(
		//	CreateProjectilePool(
		//		jMan.m_vecProjectiles,
		//		handler,
		//		type,
		//		jMan.m_fGlobalSpeed,
		//		jMan.m_bUseGlobalSpeed
		//	),
		//	jMan.m_sImagePath
		//);

		if (jMan.m_bLoop)
			pManager->EnableRepeat();

		vecManagers.push_back(std::move(pManager));
	}

	return vecManagers;
}

void ProjectileManager::Initialize(const Graphics& gfx, ConstantBuffer<Matrices>& mat)
{
	for(std::shared_ptr<Projectile>& pProjectile : m_vecProjectilePool)
		pProjectile->Initialize(gfx, mat, Sprite::Type::Projectile);
}

void ProjectileManager::InitialiseFromFile(const Graphics& gfx, ConstantBuffer<Matrices>& mat, const std::string& filename)
{
	for (std::shared_ptr<Projectile>& pProjectile : m_vecProjectilePool)
		pProjectile->Initialize(gfx, mat, filename);

	for (std::shared_ptr<ProjectilePayLoad>& pPayload: m_vecPayLoads)
		pPayload->Initialise(gfx, mat);
}

void ProjectileManager::InitialiseFromFile(const Graphics& gfx, ConstantBuffer<Matrices>& mat, const std::string& filename, Vector2f size)
{
	for (std::shared_ptr<Projectile>& pProjectile : m_vecProjectilePool)
		pProjectile->Initialize(gfx, mat, filename, size);
}

void ProjectileManager::Update( const float dt )
{
	if (!IsFinished())
		m_fDuration -= dt;
	else
	{
		// TODO : add call to spawn projectiles when duration counter is reset.
		if (m_bWillRepeat)
		{
			m_fDuration = m_fTotalDuration;
			SpawnProjectiles(m_vSpawnPosition);
		}
		else
			m_fDuration = 0.0f;
	}

	if (m_fCounter >= 0.0f)
	{
		m_fCounter -= dt;
		return;
	}

	//for (std::shared_ptr<Projectile> pProjectile : m_vecProjectilePool)
	//	pProjectile->Update(dt);

	for (std::shared_ptr<ProjectilePayLoad> pPayLoad : m_vecPayLoads)
		pPayLoad->Update(dt);
}

void ProjectileManager::Draw( ID3D11DeviceContext* context, XMMATRIX orthoMatrix )
{
	//for (std::shared_ptr<Projectile> pProjectile : m_vecProjectilePool)
	//	pProjectile->Draw(context, orthoMatrix);

	for (std::shared_ptr<ProjectilePayLoad> pPayLoad : m_vecPayLoads)
		pPayLoad->Draw(context, orthoMatrix);
}

void ProjectileManager::SetProjectilePool(std::vector<std::shared_ptr<Projectile>> vecProjectilePool, std::string sImagePath)
{
	m_fTotalDuration = m_fDelay;
	m_vecProjectilePool = vecProjectilePool;

	for (int i = 0; i < INITIAL_POOL_COUNT; i++)
	{
		std::shared_ptr <ProjectilePayLoad> pPayload = std::make_shared<ProjectilePayLoad>(vecProjectilePool, sImagePath);
		m_vecPayLoads.push_back(std::move(pPayload));
	}

	for (std::shared_ptr<Projectile> pProjectile : vecProjectilePool)
		m_fTotalDuration += pProjectile->GetDelay() + pProjectile->GetMaxLifeTime();
}

void ProjectileManager::UpdateProjectilePool(std::vector<ProjectileData::ProjectileJSON> vecProjectileJsons)
{
	std::vector<std::shared_ptr<Projectile>> vecProjectilePool;

	for (ProjectileData::ProjectileJSON pJson : vecProjectileJsons)
	{
		std::shared_ptr<Projectile> pProjectile = std::make_shared<Projectile>(pJson.m_fSpeed, m_type, pJson.m_fLifeTime);
		pProjectile->SetDirection(Vector2f(pJson.m_fAngle));
		pProjectile->SetOffSet(Vector2f(pJson.m_fX, pJson.m_fY));
		pProjectile->SetWave(pJson.m_fAngle, pJson.m_fAmplitude, pJson.m_fFrequency);

		vecProjectilePool.push_back(std::move(pProjectile));
	}

	m_vecProjectilePool = vecProjectilePool;
}

void ProjectileManager::SpawnProjectile()
{
	m_fCounter = m_fDelay;
	m_fDuration = m_fTotalDuration;

	std::shared_ptr<Projectile> pProjectile = GetFreeProjectile();

	if (pProjectile != nullptr)
		pProjectile->SpawnProjectile(m_vSpawnPosition, m_vTargetPosition, m_fLifeTime);
}

void ProjectileManager::SpawnProjectile(Vector2f vSpawnPosition, float fLifeTime)
{
	m_fCounter = m_fDelay;
	m_fDuration = m_fTotalDuration;

	std::shared_ptr<Projectile> pProjectile = GetFreeProjectile();

	if(pProjectile != nullptr)
		pProjectile->SpawnProjectile(vSpawnPosition, m_vTargetPosition, m_fLifeTime);
}

void ProjectileManager::SpawnProjectiles(Vector2f vSpawnPosition)
{
	m_fCounter = m_fDelay;
	m_fDuration = m_fTotalDuration;
	m_vSpawnPosition = vSpawnPosition;

	for (std::shared_ptr<Projectile> pProjectile : m_vecProjectilePool)
	{
		pProjectile->SetOwner(m_owner);
		pProjectile->SpawnProjectile(vSpawnPosition, m_vTargetPosition, -1.0f);
	}

	auto payload = GetProjectilePayLoad();
	if(payload != nullptr)
		payload->Fire(vSpawnPosition, m_vTargetPosition, -1.0f);
}

std::shared_ptr<Projectile> ProjectileManager::GetFreeProjectile()
{
	for (std::shared_ptr<Projectile> pProjectile : m_vecProjectilePool)
		if (!pProjectile->IsAlive())
			return pProjectile;

	return nullptr;
}

std::shared_ptr <ProjectilePayLoad> ProjectileManager::GetProjectilePayLoad()
{
	for (std::shared_ptr<ProjectilePayLoad> pPayLoad : m_vecPayLoads)
	{
		if (!pPayLoad->bIsActive)
			return pPayLoad;
	}

	return nullptr;

	/*std::vector<std::shared_ptr<Projectile>> vecProjectiles = CreateProjectilePool(
		m_pBluePrint->m_vecProjectileJsons,
		m_pBluePrint->m_pHandler,
		m_pBluePrint->m_sType,
		m_pBluePrint->m_fGlobalSpeed,
		m_pBluePrint->m_bUseGlobalSpeed
	);

	std::shared_ptr<ProjectilePayLoad> pPayLoad = std::make_shared<ProjectilePayLoad>(vecProjectiles);

	int iLastIndex = m_vecProjectilePayLoad.size();

	for (int i = 0; i < INITIAL_POOL_COUNT; i++)
		m_vecProjectilePayLoad.push_back(pPayLoad);

	return m_vecProjectilePayLoad[iLastIndex];
	*/
}