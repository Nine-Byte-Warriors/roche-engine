#include "stdafx.h"
#include "AISeek.h"
#include "Vector2f.h"

using namespace AILogic;

void AISeek::Update(const float dt)
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

	// Multiply the direction by the speed
	Vector2f vVelocity = vDirection.Multiply(m_pAgent->GetSpeed());

	// Multiply the velocity by time
	Vector2f vStep = vVelocity.Multiply(dt);
	
	// Calculate the new agent's position
	//Vector2f vResultPosition = vAgentPos.Add(vStep);

	// Apply the new position to the agent's GameObject
	//m_pAgent->SetPosition(vResultPosition.x, vResultPosition.y, 0.0f);
	m_pAgent->AdjustPosition(vStep.x, vStep.y, 0.0f);
}