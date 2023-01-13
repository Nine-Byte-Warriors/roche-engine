#include "stdafx.h"
#include "AIStateMachine.h"
#include "AISeek.h"
#include "AIIdle.h"
#include "AIFlee.h"

using namespace AILogic;

void AIStateMachine::UpdateMachine(float fDelta)
{
    if (m_vecStates.empty()) return;

    // Store active states from previous iteration
    m_vecLastActiveStates = m_vecActiveStates;

    // Clear list of active states
    m_vecActiveStates.clear();

	for (AIState* pState : m_vecStates)
	{
        if (pState->CalculateActivation() == 0)
            pState->Exit();
        else
			m_vecActiveStates.push_back(pState);
	}
	
    if (!m_vecActiveStates.empty()) 
    {
        for (AIState* pState : m_vecActiveStates)
            if(IsActive(pState))
				pState->Update(fDelta);
            else
				pState->Enter();
    }
}

void AIStateMachine::AddState(AIState* pStateToAdd)
{
    for (AIState* pState : m_vecStates)
		if (pState == pStateToAdd) return;
	
    m_vecStates.push_back(pStateToAdd);
}

bool AIStateMachine::IsActive(AIState* pStateToAdd)
{
	for (AIState* pState : m_vecLastActiveStates)
		if (pState == pStateToAdd) return true;

	return false;
}

AIState* AIStateMachine::NewState(AIStateTypes fType)
{
	switch (fType)
	{
	case AIStateTypes::Idle: return new AIIdle(m_pAgent);
	case AIStateTypes::Seek: return new AISeek(m_pAgent);
	case AIStateTypes::Flee: return new AIFlee(m_pAgent);
	default: return nullptr;
	}
}