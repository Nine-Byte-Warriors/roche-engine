#include "stdafx.h"
#include "Agent.h"

void Agent::Initialize( const std::shared_ptr<Transform>& transform )
{
	m_fSpeed = 10.0f;
	m_transform = transform;
	
	m_pStateMachine = new /*AILogic::*/AIStateMachine(this);
	/*AILogic::*/AIState* pSeekState = m_pStateMachine->NewState(/*AILogic::*/AIStateTypes::Seek);
	pSeekState->SetBounds(1.0f, 0.0f);
	pSeekState->SetActivation(0.0f);
	m_vecStates.push_back(pSeekState);
	
	/*AILogic::*/AIState* pIdleState = m_pStateMachine->NewState(/*AILogic::*/AIStateTypes::Idle);
	pIdleState->SetActivation(0.0f);
	m_vecStates.push_back(pIdleState);

	/*AILogic::*/AIState* pFleeState = m_pStateMachine->NewState(/*AILogic::*/AIStateTypes::Flee);
	pFleeState->SetBounds(1.0f, 0.0f);
	pFleeState->SetActivation(1.0f);
	m_vecStates.push_back(pFleeState);
	
	AddToEvent();
}

void Agent::Update(float dt)
{
	m_pStateMachine->Clear();
	
	for (/*AILogic::*/AIState* pState : m_vecStates)
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