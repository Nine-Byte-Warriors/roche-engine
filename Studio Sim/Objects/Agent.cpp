#include "stdafx.h"
#include "Agent.h"

void Agent::Initialize( const std::shared_ptr<Transform>& transform )
{
	m_fSpeed = 10.0f;
	m_transform = transform;
	
	// TODO: replace with values from JSON 
	m_pStateMachine = new AIStateMachine(this);
	AIState* pSeekState = m_pStateMachine->NewState(AIStateTypes::Seek);
	pSeekState->SetBounds(1.0f, 0.0f);
	pSeekState->SetActivation(0.0f);
	m_vecStates.push_back(pSeekState);
	
	AIState* pIdleState = m_pStateMachine->NewState(AIStateTypes::Idle);
	pIdleState->SetActivation(0.0f);
	m_vecStates.push_back(pIdleState);

	AIState* pFleeState = m_pStateMachine->NewState(AIStateTypes::Flee);
	pFleeState->SetBounds(1.0f, 0.0f);
	pFleeState->SetActivation(1.0f);
	m_vecStates.push_back(pFleeState);
	
	AddToEvent();
}

void Agent::Update(float dt)
{
	m_pStateMachine->Clear();
	
	for (AIState* pState : m_vecStates)
		m_pStateMachine->AddState(pState);
	
	m_pStateMachine->UpdateMachine(dt);
}

void Agent::HandleEvent(Event* event)
{
	switch (event->GetEventID())
	{
	case EVENTID::PlayerPosition:
		m_vTargetPos = *(Vector2f*)event->GetData();
		break;
	default:
		break;
	}
}