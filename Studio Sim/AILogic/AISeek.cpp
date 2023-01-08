#include "stdafx.h"
#include "AISeek.h"
#include "Vector2f.h"

using namespace AILogic;

void AISeek::Update(const float dt)
{
	//if (m_pTarget == nullptr) return;

	// Get the direction to the target
	Vector2f vTargetDir = Vector2f::Left();	// this is to be replaced by the actual direction to the target
	Vector2f vDirection = m_vPosition.Direction(vTargetDir);

	// Normalize the direction
	vDirection.Normalised();

	// Multiply the direction by the speed
	vDirection.Multiply(m_fSpeed);

	// Set the agent's velocity to the direction
	m_pAgent->SetVelocity(vDirection);
}