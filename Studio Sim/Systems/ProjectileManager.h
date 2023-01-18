#pragma once
#ifndef PROJECTILEMANAGER_H
#define PROJECTILEMANAGER_H

#include "Projectile.h"
#include "EventSystem.h"
#include "ProjectileData.h"

class ProjectileManager : public Listener
{
public:
	// NOTE: A manager per projectile type.
	ProjectileManager();
	~ProjectileManager() {}
	
	void Initialize(const Graphics& gfx, ConstantBuffer<Matrices>& mat);
	void InitialiseFromFile(const Graphics& gfx, ConstantBuffer<Matrices>& mat, const std::string& filename, const int iCount);
	void Update(const float dt);
	void Draw( ID3D11DeviceContext* context, XMMATRIX orthoMatrix );

	void ResetPool(ProjectileData::ProjectileJSON projectileData, const Graphics& gfx, ConstantBuffer<Matrices>& mat);
	void ResetPool(std::vector<ProjectileData::ManagerJSON> vecManagerData, const Graphics& gfx, ConstantBuffer<Matrices>& mat);

	void SpawnProjectile(Vector2f vSpawnPosition, float fLifeTime);
	void SpawnProjectiles();
	
	//inline std::shared_ptr<Projectile> GetProjectile() const noexcept { return m_projectile; } // single projectile
	//inline std::shared_ptr<Projectile> GetProjectiles() const noexcept { return m_projectiles; } // multiple projectiles
	
	void AddToEvent() noexcept;
	void HandleEvent(Event* event) override;
	
private:
	void SpawnProjectile();
	std::shared_ptr<Projectile> GetFreeProjectile();

	float m_fLifeTime;
	Vector2f m_vSpawnPosition;
	Vector2f m_vTargetPosition;
	
	std::vector<std::shared_ptr<Projectile>> m_vecProjectilePool;
};

#endif // !PROJECTILEMANAGER_H