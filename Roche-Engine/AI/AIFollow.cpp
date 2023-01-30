#include "stdafx.h"
#include "AIFollow.h"

using namespace AILogic;

void AIFollow::Update(const float dt)
{
	// Get agent position from agent's GameObject
	Vector2f vAgentPosition = m_pAgent->GetPhysics()->GetTransform()->GetPosition();

	// Get target position
	Vector2f vTargetPosition = m_pAgent->GetTargetPosition();

	// Get the direction from the target
	Vector2f vDirection = vAgentPosition
		.DirectionTo(vTargetPosition)
		.Normalised();

	float fDistanceToTarget = vAgentPosition.Distance(vTargetPosition);

	if (m_fFollowRange < fDistanceToTarget) 
		// Apply the directional force and speed
		m_pAgent->GetPhysics()->AddForce(vDirection * m_pAgent->GetSpeed());

	if(m_fRepulseRange > fDistanceToTarget && m_bKeepRange == true)
		// Apply the directional force and speed
		m_pAgent->GetPhysics()->AddForce(vDirection * -m_pAgent->GetSpeed());

	// Update the physics model
	m_pAgent->GetPhysics()->Update(dt);


}

void AIFollow::GetFollowParams()
{
	FollowParams* pParams = (FollowParams*)m_params;
	m_fFollowRange = pParams->fFollowRange;
	m_fRepulseRange = pParams->fRepulseRange;
	m_bKeepRange = pParams->bKeepRange;
}
