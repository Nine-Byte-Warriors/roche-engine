#pragma once
#ifndef AIPATROL_H
#define AIPATROL_H

#include "AIState.h"
#include "Agent.h"
#include "RND.h"

namespace AILogic
{
	class AIPatrol : public AIState
	{
	public:
		AIPatrol(Agent* pAgent) : AIState(pAgent) {};
		virtual ~AIPatrol() = default;

		virtual void Update(const float dt) override;
		virtual void Enter() override;
		virtual void Exit() override {};

		virtual std::vector<Vector2f> GetWaypoints() const override { return m_vecWaypoints; }
		virtual int GetCurrentWaypointIndex() const noexcept { return m_iCurrentWaypoint; }

	private:
		void GetPatrolParams();

		inline bool IsWaypointInRange(Vector2f vAgentPos) noexcept 
			{ return m_fSensingRange >= vAgentPos.Distance(m_vecWaypoints[m_iCurrentWaypoint]); }
		inline int GetRandomWaypoint() const noexcept { return RND::Get(m_iWaypointCount); }
		int GetNextWaypoint() noexcept;

		float m_fSensingRange;
		float m_fWaypointDistance;
		
		int m_iCurrentWaypoint, m_iWaypointCount;
		std::vector<Vector2f> m_vecWaypoints;
		
		PatrolType m_ePatrolType;
	};
}

#endif // !AIPATROL_H