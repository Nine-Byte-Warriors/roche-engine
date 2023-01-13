#pragma once
#ifndef PROJECTILEMANAGER_H
#define PROJECTILEMANAGER_H

#include "Projectile.h"

class ProjectileManager : public Listener
{
public:
	ProjectileManager();
	void Initialize(const Graphics& gfx, ConstantBuffer<Matrices>& mat);
	void Update(const float dt);
	
	void AddToEvent() noexcept;
	void HandleEvent(Event* event) override;
	
private:
	//std::vector<std::shared_ptr<Projectile>> m_vProjectiles;
	std::shared_ptr<Projectile> m_pProjectile;	//DEBUG: should be a collection from a bullet pool
	float m_fLifeTime;
	Vector2f m_vSpawnPosition;
	Vector2f m_vTargetPosition;
};

#endif // !PROJECTILEMANAGER_H