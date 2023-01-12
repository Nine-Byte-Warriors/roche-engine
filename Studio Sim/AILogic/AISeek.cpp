#include "stdafx.h"
#include "AISeek.h"

//using namespace AILogic;

void AISeek::Update(const float dt)
{
	// Get agent position from agent's GameObject
	Vector2f vAgentPos = m_pAgent->GetPhysics()->GetTransform()->GetPosition();

	// Get target position
	Vector2f vTargetPosition = m_pAgent->GetTargetPosition();

	// Get the direction to the target
	Vector2f vDirection = vAgentPos.Direction(vTargetPosition);

	// Normalize the direction
	Vector2f vNormDir = vDirection.Normalised();
	
	// Apply the directional force and speed
	m_pAgent->GetPhysics()->AddForce( vNormDir * m_pAgent->GetSpeed() );
	
	// Update the physics model
	m_pAgent->GetPhysics()->Update( dt );
}