#include "stdafx.h"
#include "AIWander.h"

using namespace AILogic;

void AIWander::Update(const float dt)
{
	// Get agent position from agent's GameObject
	Vector2f vAgentPos = m_pAgent->GetPhysics()->GetTransform()->GetPosition();

	Vector2f vVelocity = m_pAgent->GetPhysics()->GetVelocity();
	vVelocity = vVelocity.Normalised();

	// Get the direction from the target
	Vector2f vDirection = DelayCount(dt)
		? vVelocity.Rotate(RND::Get(m_fWanderAngle))
		: vVelocity;

	// Apply the directional force and speed
	m_pAgent->GetPhysics()->AddForce(vDirection * m_pAgent->GetSpeed());

	// Update the physics model
	m_pAgent->GetPhysics()->Update(dt);
}

void AIWander::GetWanderParams()
{
	WanderParams* pParams = (WanderParams*)m_params;
	m_fWanderAngle = pParams->fWanderAngle;
	m_fMaxDelay = pParams->fWanderDelay;
}

bool AIWander::DelayCount(const float dt)
{
	m_fDelayCount -= dt;

	if (m_fDelayCount <= 0.0f)
	{
		m_fDelayCount = m_fMaxDelay;
		return true;
	}

	return false;
}