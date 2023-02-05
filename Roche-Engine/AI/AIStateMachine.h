#pragma once
#ifndef AISTATEMACHINE_H
#define AISTATEMACHINE_H

#include "AIState.h"

namespace AILogic
{
	enum class AIStateTypes
	{
		Idle = 0,
		Seek = 1,
		Flee = 2,
		Patrol = 3,
		Follow = 4,
		Wander = 5,
		Fire = 6,
	};
	
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

		void Clear() { if(m_vecStates.size() > 0) m_vecStates.clear(); } // temporary. should be done differently
		AIState* NewState(AIStateTypes fType); // tmeporary. should be done differently and elsewhere
		
	protected:
		std::vector<AIState*> m_vecStates;
		std::vector<AIState*> m_vecActiveStates;
		std::vector<AIState*> m_vecLastActiveStates;

	private:
		
	};
}
	
#endif // AISTATEMACHINE_H