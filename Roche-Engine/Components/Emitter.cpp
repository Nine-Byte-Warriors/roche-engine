#include "stdafx.h"
#include "Emitter.h"

Emitter::Emitter(const std::shared_ptr<ProjectileManager>& pProjectileManager, float fDelay)
{
	m_fDelay = 0.1f;
	m_fMaxDelay = fDelay;
	m_pProjectileManager = pProjectileManager;
}

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
	m_pProjectileManager->SetTargetPosition(m_vTargetPosition);
	m_pProjectileManager->SpawnProjectiles(m_vSpawnPosition);
}
