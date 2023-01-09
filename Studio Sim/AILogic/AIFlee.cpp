#include "stdafx.h"
#include "AIFlee.h"
#include "Vector2f.h"

using namespace AILogic;

void AIFlee::Update(const float dt)
{
	// Get agent position from agent's GameObject
	Vector2f vAgentPos = m_pAgent->GetPositionVector2f();
	
	// Get target position
	// this is to be replaced by the actual direction to the target
	Vector2f vTargetDir = Vector2f::Left();
	Vector2f vTargetPosition = vAgentPos.Add(vTargetDir);

	// Get the direction to the target
	Vector2f vDirection = vAgentPos.Direction(vTargetPosition);

	// Normalize the direction
	Vector2f vNormDir = vDirection.Normalised();

	// Get the distance to the target
	float distance = vAgentPos.Distance(vTargetPosition);

	// Calculate the desired velocity
	Vector2f vVelocity = vDirection.Multiply(m_pAgent->GetSpeed());

	// Calculate the steering force
	Vector2f vForce = vVelocity.Subtract(vDirection);

	// Set the steering force
	m_pAgent->AdjustPosition(vForce.x, vForce.y, 0.0f);
}