#pragma once
#ifndef PROJECTILEMANAGER_H
#define PROJECTILEMANAGER_H

#include "Projectile.h"
#include "EventSystem.h"
#include "ProjectileData.h"

struct ProjectilePayLoad
{
	std::vector<std::shared_ptr<Projectile>> m_vecProjectilePool;
	bool m_bActive;
	inline bool IsActive() { return m_bActive == true; }
};

class ProjectileManager : public Listener
{
public:
	ProjectileManager();
	~ProjectileManager();

	void Initialize(const Graphics& gfx, ConstantBuffer<Matrices>& mat);
	void InitialiseFromFile(const Graphics& gfx, ConstantBuffer<Matrices>& mat, const std::string& filename);
	void Update(const float dt);
	void Draw( ID3D11DeviceContext* context, XMMATRIX orthoMatrix );

	void SetProjectilePool(std::vector<std::shared_ptr<Projectile>> vecProjectilePool) { m_vecProjectilePool = vecProjectilePool; }
	inline void SetDelay(const float fDelay) noexcept { m_fDelay = fDelay; }

	void UpdatePattern(std::string filepath);

	void SpawnProjectile(Vector2f vSpawnPosition, float fLifeTime);
	void SpawnProjectiles(Vector2f vSpawnPosition);

	inline std::vector<std::shared_ptr<Projectile>> GetProjector() const noexcept { return m_vecProjectilePool; };
	void SetProjectileScaleInit(const float fScaleX, const float fScaleY);
	void SetProjectileMaxFrame(const float fFrameX, const float fFrameY);
	
	void AddToEvent() noexcept;
	void RemoveFromEvent() noexcept;
	void HandleEvent(Event* event) override;

private:
	void SpawnProjectile();
	std::shared_ptr<Projectile> GetFreeProjectile();
	std::shared_ptr<ProjectilePayLoad> GetProjectilePayLoad();

	std::vector<std::shared_ptr<Projectile>> CreateProjectilePool(std::vector<ProjectileData::ProjectileJSON> vecProjectileJsons);
	void UpdateProjectilePayLoad(std::vector<ProjectileData::ProjectileJSON> vecProjectileJsons);

	float m_fLifeTime;
	float m_fDelay;
	float m_fCounter;
	Vector2f m_vSpawnPosition;
	Vector2f m_vTargetPosition;
	std::vector<std::shared_ptr<ProjectilePayLoad>> m_vecProjectilePayLoads;

	std::vector<ProjectileData::ManagerJSON> m_vecManagers;
};

#endif // !PROJECTILEMANAGER_H