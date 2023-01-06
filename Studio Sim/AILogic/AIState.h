#pragma once
#ifndef AISTATE_H
#define AISTATE_H

#include "Vector2f.h"	// TODO: to be removed

namespace AILogic
{
	struct Agent
	{
		// TODO: replace with an enemy, boss or game object
		Vector2f m_pPosition;
		Vector2f m_vVelocity;
		
		void SetVelocity(Vector2f vVelocity) { m_vVelocity = vVelocity; }
	};
	
	class AIState
	{
	public:
		AIState(Agent* pAgent) : m_pAgent(pAgent) {};
		~AIState() = default;

		virtual void Update(const float dt) {}
		virtual void Enter() {}
		virtual void Exit() {}
		
		virtual float CalculateActivation() { return m_fActivationLevel; }

		virtual void SetBounds(float fUpper, float fLower) { m_fUpper = fUpper; m_fLower = fLower; }
		virtual void CheckBounds() { CheckLowerBound(); CheckUpperBound(); }
		virtual void CheckLowerBound() { if (m_fActivationLevel < m_fLower) m_fActivationLevel = m_fLower; }
		virtual void CheckUpperBound() { if (m_fActivationLevel > m_fUpper) m_fActivationLevel = m_fUpper; }
		
		Agent* m_pAgent;
		
		float m_fActivationLevel;
		
	private:
		float m_fUpper;
		float m_fLower;
	};
}

#endif // AISTATE_H