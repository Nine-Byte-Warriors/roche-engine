#pragma once
#ifndef EMITTER_H
#define EMITTER_H

#include "Vector2f.h"
#include "ProjectileManager.h"
#include "EventSystem.h"

class Emitter
{
public:
	Emitter(const std::shared_ptr<ProjectileManager>& pProjectileManager, float fDelay = 0.1f);
	~Emitter() {}

	inline void SetSpawnPosition(const Vector2f vSpawnPosition) noexcept { m_vSpawnPosition = vSpawnPosition; }
	inline void SetTargetPosition(const Vector2f vTargetPosition) noexcept { m_vTargetPosition = vTargetPosition; }
	inline void SetProjectileManager(std::shared_ptr<ProjectileManager>& pProjectileManager) noexcept { m_pProjectileManager = pProjectileManager; }
	
	void Update(float dt);
	void EmitProjectile();
	
private:
	Vector2f m_vSpawnPosition;
	Vector2f m_vTargetPosition;
	
	float m_fDelay;
	float m_fMaxDelay;

	std::shared_ptr<ProjectileManager> m_pProjectileManager;
};
#endif // !EMITTER_H
