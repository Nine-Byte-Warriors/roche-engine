#include "stdafx.h"
#include "AIFlee.h"

using namespace AILogic;

void AIFlee::Update(const float dt)
{
	// Get agent position from agent's GameObject
	Vector2f vAgentPos = m_pAgent->GetPhysics()->GetTransform()->GetPosition();
	
	// Get target position
	Vector2f vTargetPosition = m_pAgent->GetTargetPosition();

	// Get the direction from the target
	Vector2f vDirection = vTargetPosition
		.DirectionTo(vAgentPos)
		.Normalised();

	// Apply the directional force and speed
	m_pAgent->GetPhysics()->AddForce(vDirection * m_pAgent->GetSpeed() );
	
	// Update the physics model
	m_pAgent->GetPhysics()->Update( dt );
}