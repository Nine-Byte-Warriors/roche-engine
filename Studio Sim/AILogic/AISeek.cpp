#include "stdafx.h"
#include "AISeek.h"
#include "Vector2f.h"

using namespace AILogic;

void AISeek::Update(const float dt)
{
	if (m_pTarget == nullptr) return;

	// Get the direction to the target
	Vector2f vDirection = Vector2f::Distance(m_pTarget->m_pPosition, m_pAgent->m_pPosition);

	// Normalize the direction
	vDirection.Normalised();

	// Multiply the direction by the speed
	vDirection.Multiply(m_fSpeed);

	// Set the agent's velocity to the direction
	m_pAgent->m_vVelocity = vDirection;
}