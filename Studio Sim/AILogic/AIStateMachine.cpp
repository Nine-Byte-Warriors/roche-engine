#include "stdafx.h"
#include "AIStateMachine.h"

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
        pState->m_pAgent = m_pAgent;
		
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