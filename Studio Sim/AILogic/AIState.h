#pragma once
#ifndef AISTATE_H
#define AISTATE_H

#include "Vector2f.h"	

class Agent;

namespace AILogic
{
	class AIState
	{
	public:
		AIState(Agent* pAgent) : m_pAgent(pAgent) {};
		~AIState() = default;

		virtual void Update(const float dt) {}
		virtual void Enter() {}
		virtual void Exit() {}
		
		virtual float CalculateActivation() { return m_fActivationLevel; }
		virtual void SetActivation(const float fActivation) { m_fActivationLevel = fActivation; }
		virtual void SetBounds(const float fUpper, const float fLower) { m_fUpper = fUpper; m_fLower = fLower; }
		virtual void CheckBounds() { CheckLowerBound(); CheckUpperBound(); }
		virtual void CheckLowerBound() { if (m_fActivationLevel < m_fLower) m_fActivationLevel = m_fLower; }
		virtual void CheckUpperBound() { if (m_fActivationLevel > m_fUpper) m_fActivationLevel = m_fUpper; }
		
		float m_fActivationLevel;
		
	protected:
		Agent* m_pAgent;
		
	private:
		float m_fUpper;
		float m_fLower;
	};
}

#endif // AISTATE_H