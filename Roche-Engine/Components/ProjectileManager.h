
#pragma once
#ifndef PROJECTILEMANAGER_H
#define PROJECTILEMANAGER_H

#include "Projectile.h"
#include "ProjectileData.h"
#include "ProjectilePayLoad.h"
#include "CollisionHandler.h"

#define PATTERN_FOLDER_PATH "Resources\\Patterns\\"
#define INITIAL_POOL_COUNT 10

class ProjectileManager
{
public:
	ProjectileManager(Projectile::ProjectileOwner owner, std::string type);
	~ProjectileManager();

	static std::vector<std::shared_ptr<Projectile>> CreateProjectilePool(std::vector<ProjectileData::ProjectileJSON> vecProjectileJsons, CollisionHandler* handler, std::string type, float fGlobalSpeed, bool bUseGlobalSpeed);
	static std::vector<std::shared_ptr<ProjectileManager>> GenerateManagers(const std::string filepath, CollisionHandler* handler, std::string type);

	void Initialize(const Graphics& gfx, ConstantBuffer<Matrices>& mat);
	void InitialiseFromFile(const Graphics& gfx, ConstantBuffer<Matrices>& mat, const std::string& filename);
	void InitialiseFromFile(const Graphics& gfx, ConstantBuffer<Matrices>& mat, const std::string& filename, Vector2f size);
	void Update(const float dt);
	void Draw( ID3D11DeviceContext* context, XMMATRIX orthoMatrix );

	void SetProjectilePool(std::vector<std::shared_ptr<Projectile>> vecProjectilePool, std::string sImagePath);
	inline void SetDelay(const float fDelay) noexcept { m_fDelay = fDelay; }
	inline void SetTargetPosition(const Vector2f vTargetPosition) noexcept
	{ 
		Vector2f vOffSet = m_vecProjectilePool[0]->GetSprite()->GetWidthHeight() / 2;
		m_vTargetPosition = vTargetPosition - vOffSet;
	}

	void SpawnProjectile(Vector2f vSpawnPosition, float fLifeTime);
	void SpawnProjectiles(Vector2f vSpawnPosition);

	inline std::vector<std::shared_ptr<Projectile>> GetProjector() const noexcept { return m_vecProjectilePool; };
	inline bool IsFinished() const noexcept { return m_fDuration <= 0.0f; }
	inline void EnableRepeat() noexcept { m_bWillRepeat = true; }

	inline void SetOwner(const Projectile::ProjectileOwner owner) noexcept { m_owner = owner; }

private:
	void SpawnProjectile();
	std::shared_ptr<Projectile> GetFreeProjectile();
	std::shared_ptr<ProjectilePayLoad> GetProjectilePayLoad();

	void UpdateProjectilePool(std::vector<ProjectileData::ProjectileJSON> vecProjectileJsons);

	float m_fLifeTime;
	float m_fDelay;
	float m_fCounter;
	float m_fTotalDuration;
	float m_fDuration;
	bool m_bWillRepeat;
	Vector2f m_vSpawnPosition;
	Vector2f m_vTargetPosition;
	std::vector<std::shared_ptr<Projectile>> m_vecProjectilePool;
	std::vector<std::shared_ptr<ProjectilePayLoad>> m_vecPayLoads;

	std::vector<ProjectileData::ManagerJSON> m_vecManagers;
	Projectile::ProjectileOwner m_owner;
	std::string m_type;
};

#endif // !PROJECTILEMANAGER_H