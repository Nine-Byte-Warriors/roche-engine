#pragma once
#ifndef PROJECTILEMANAGER_H
#define PROJECTILEMANAGER_H

#include "Projectile.h"
#include "ProjectileData.h"

#define PATTERN_FOLDER_PATH "Resources\\Patterns\\"
#define INITIAL_POOL_COUNT 10

class ProjectileManager
{
public:
	ProjectileManager(Projectile::ProjectileOwner owner = Projectile::ProjectileOwner::None);
	~ProjectileManager();

	static std::vector<std::shared_ptr<Projectile>> CreateProjectilePool(std::vector<ProjectileData::ProjectileJSON> vecProjectileJsons, float fGlobalSpeed, bool bUseGlobalSpeed);
	static std::vector<std::shared_ptr<ProjectileManager>> GenerateManagers(const std::string filepath);

	void Initialize(const Graphics& gfx, ConstantBuffer<Matrices>& mat);
	void InitialiseFromFile(const Graphics& gfx, ConstantBuffer<Matrices>& mat, const std::string& filename);
	void InitialiseFromFile(const Graphics& gfx, ConstantBuffer<Matrices>& mat, const std::string& filename, Vector2f size);
	void Update(const float dt);
	void Draw( ID3D11DeviceContext* context, XMMATRIX orthoMatrix );

	void SetProjectilePool(std::vector<std::shared_ptr<Projectile>> vecProjectilePool);
	inline void SetDelay(const float fDelay) noexcept { m_fDelay = fDelay; }
	inline void SetTargetPosition(const Vector2f vTargetPosition) noexcept
	{ 
		Vector2f vOffSet = m_vecProjectilePool[0]->GetSprite()->GetWidthHeight() / 2;
		m_vTargetPosition = vTargetPosition - vOffSet;
	}

	void UpdatePattern(std::string filepath);

	void SpawnProjectile(Vector2f vSpawnPosition, float fLifeTime);
	void SpawnProjectiles(Vector2f vSpawnPosition);

	inline std::vector<std::shared_ptr<Projectile>> GetProjector() const noexcept { return m_vecProjectilePool; };
	inline bool IsFinished() const noexcept { return m_fDuration <= 0.0f; }
	inline void EnableRepeat() noexcept { m_bWillRepeat = true; }

	inline void SetOwner(const Projectile::ProjectileOwner owner) noexcept { m_owner = owner; }

private:
	void SpawnProjectile();
	std::shared_ptr<Projectile> GetFreeProjectile();

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

	std::vector<ProjectileData::ManagerJSON> m_vecManagers;
	Projectile::ProjectileOwner m_owner;
};

#endif // !PROJECTILEMANAGER_H