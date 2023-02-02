#include "stdafx.h"
#include "Emitter.h"

void Emitter::Update(float dt)
{
	m_fDelay -= dt;
	if (m_fDelay <= 0.0f)
	{
		EmitProjectile();
		m_fDelay = m_fMaxDelay;
	}
	m_pProjectileManager->Update(dt);
}

void Emitter::EmitProjectile()
{
	m_pProjectileManager->SetTargetPosition(m_vTargetPosition);
	m_pProjectileManager->SpawnProjectiles(m_vSpawnPosition);
}

