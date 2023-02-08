#pragma once
#ifndef EMITTER_H
#define EMITTER_H

#include "ProjectileManager.h"

class Emitter
{
public:
	Emitter(const std::vector<std::shared_ptr<ProjectileManager>>& vecProjectileManagers, float fDelay = 0.1f) :
		m_vecProjectileManagers(vecProjectileManagers),
		m_fDelay(0.1f), m_fMaxDelay(fDelay)
	{};
	~Emitter() {}

	inline void SetSpawnPosition(const Vector2f vSpawnPosition) noexcept { m_vSpawnPosition = vSpawnPosition; }
	inline void SetTargetPosition(const Vector2f vTargetPosition) noexcept { m_vTargetPosition = vTargetPosition; }

	void Update(float dt);
	void EmitProjectile();

private:
	Vector2f m_vSpawnPosition;
	Vector2f m_vTargetPosition;

	float m_fDelay;
	float m_fMaxDelay;

	std::vector<std::shared_ptr<ProjectileManager>> m_vecProjectileManagers;
};
#endif // !EMITTER_H
