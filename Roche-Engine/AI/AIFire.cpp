#include "stdafx.h"
#include "AIFire.h"

using namespace AILogic;

void AIFire::Update(const float dt)
{
	m_fFireTimer -= dt;
	
	if (m_fFireTimer < 0.0f)
	{
		m_fFireTimer = m_fFireRate;
		
		m_pEmitter->SetSpawnPosition(m_pAgent->GetPhysics()->GetTransform()->GetPosition());
		m_pEmitter->SetTargetPosition(m_pAgent->GetTargetPosition());
		m_pEmitter->EmitProjectile();
	}
}

void AIFire::Enter()
{
	m_pEmitter = m_pAgent->GetEmitter();
}