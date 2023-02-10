#include "stdafx.h"
#include "Emitter.h"

void Emitter::Update(float dt)
{
	m_fDelay -= dt;
	if (m_fDelay <= 0.0f)
	{
		//EmitProjectile();
		m_fDelay = m_fMaxDelay;
	}
}

void Emitter::EmitProjectile()
{
	for (std::shared_ptr<ProjectileManager>& pProjectileManager : m_vecProjectileManagers)
		pProjectileManager->SpawnProjectiles(m_vSpawnPosition, m_vTargetPosition);
}
