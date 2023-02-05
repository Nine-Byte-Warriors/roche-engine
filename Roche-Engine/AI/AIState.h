#pragma once
#ifndef AISTATE_H
#define AISTATE_H

#include "Vector2f.h"

class Agent;

namespace AILogic
{
	enum class PatrolType
	{
		Loop,
		Random
	};

	// TODO: find a better way to pass parameters to specific states
	struct PatrolParams
	{
		float fSensingRange;
		int iWaypointCount;
		float fDistanceToWaypoint;
		PatrolType ePatrolType;
	};

	struct FollowParams
	{
		float fFollowRange;
		float fRepulseRange;
		bool bKeepRange;
	};

	struct WanderParams
	{
		float fWanderAngle;
		float fWanderDelay;
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
		virtual void SetActivation(const float fActivation) { m_fActivationLevel = fActivation; }
		virtual void SetBounds(const float fUpper, const float fLower) { m_fUpper = fUpper; m_fLower = fLower; }
		virtual void CheckBounds() { CheckLowerBound(); CheckUpperBound(); }
		virtual void CheckLowerBound() { if (m_fActivationLevel < m_fLower) m_fActivationLevel = m_fLower; }
		virtual void CheckUpperBound() { if (m_fActivationLevel > m_fUpper) m_fActivationLevel = m_fUpper; }
		
		virtual void SetParams(void* params) { m_params = params; }
		
		// DEBUG
		virtual std::vector<Vector2f> GetWaypoints() const { return std::vector<Vector2f>(); }
		virtual int GetCurrentWaypointIndex() const noexcept { return 0; }
		
		float m_fActivationLevel;
		
	protected:
		Agent* m_pAgent;
		void* m_params;
		
	private:
		float m_fUpper;
		float m_fLower;
	};
}

#endif // AISTATE_H