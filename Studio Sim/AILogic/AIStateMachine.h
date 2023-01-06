#pragma once
#ifndef AISTATEMACHINE_H
#define AISTATEMACHINE_H

#include <vector>

#include "AIState.h"

namespace AILogic
{
	class AIStateMachine : public AIState
	{
	public:
		AIStateMachine(Agent* fAgent) :	AIState(fAgent)
		{
			m_vecStates.clear();
			m_vecActiveStates.clear();
			m_vecLastActiveStates.clear();
		}
		virtual ~AIStateMachine() = default;

		virtual void UpdateMachine(float fDelta);
		virtual void AddState(AIState* pState);
		virtual bool IsActive(AIState* pState);
		virtual void Reset() { for (int i = 0; i < m_vecStates.size(); i++)	m_vecStates[i]->Exit();	}
		
	protected:
		std::vector<AIState*> m_vecStates;
		std::vector<AIState*> m_vecActiveStates;
		std::vector<AIState*> m_vecLastActiveStates;

	private:

	};
}
	
#endif // AISTATEMACHINE_H