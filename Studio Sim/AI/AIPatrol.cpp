#include "stdafx.h"
#include "AIPatrol.h"

using namespace AILogic;

// NOTE: If the agent is to perform another action instead of continuing to patrol,
//		the CalculateActivation() function will need to account for that by reducing 
//		the activation value accordingly.
//		For example, if the agent is to attack the player, the activation value for 
//		patrolling will need to be reduced by a certain amount. In addition, the 
//		activation value for attacking will need to be increased so that the agent
//		will be more likely to attack the player than to continue patrolling.

void AIPatrol::Update(const float dt)
{
	// Get agent position from agent's GameObject
	Vector2f vAgentPos = m_pAgent->GetPhysics()->GetTransform()->GetPosition();
	
	// get target waypoint position
	Vector2f vTargetPos = m_vecWaypoints[m_iCurrentWaypoint];
	
	// get normalised direction to target waypoint
	Vector2f vDirection = vTargetPos
		.DirectionTo(vAgentPos)
		.Normalised();

	// apply the directional force and speed
	m_pAgent->GetPhysics()->AddForce(vDirection * m_pAgent->GetSpeed());

	// update the physics model
	m_pAgent->GetPhysics()->Update(dt);

	// get distance to target waypoint
	float fDistance = vAgentPos.Distance(vTargetPos);

	// if the agent is within a certain range of the target waypoint position,
	if (IsWaypointInRange(vAgentPos))
		// get the next waypoint
		m_iCurrentWaypoint = m_ePatrolType == PatrolType::Random
			? GetRandomWaypoint()
			: GetNextWaypoint();
}

void AIPatrol::Enter()
{
	GetPatrolParams();

	// Get target position
	Vector2f vTargetPosition = m_pAgent->GetTargetPosition();

	// calculate the angle to each waypoint by dividing 360 by the number of waypoints
	float fAngle = 360.0f / m_iWaypointCount;
	
	// calculate the position of each waypoint by using the angle and range
	for (int i = 0; i < m_iWaypointCount; ++i)
	{
		// calculate the position of the waypoint
		Vector2f vWaypointPosition = vTargetPosition + Vector2f(fAngle * i) * m_fSensingRange;

		// add the waypoint to the list of waypoints
		m_vecWaypoints.push_back(vWaypointPosition);
	}
	
	// set current waypoint index to a random waypoint from the list of waypoints
	m_iCurrentWaypoint = RND::Get(0, m_iWaypointCount - 1);
}

void AIPatrol::GetPatrolParams()
{
	m_ePatrolType = m_params.ePatrolType;
	m_fSensingRange = m_params.fSensingRange;
	m_fWaypointDistance = m_params.fDistanceToWaypoint;
	m_iWaypointCount = m_params.iWaypointCount;
}

int AIPatrol::GetNextWaypoint() noexcept
{
	m_iCurrentWaypoint++;

	if (m_iCurrentWaypoint >= m_vecWaypoints.size())
		m_iCurrentWaypoint = 0;

	return m_iCurrentWaypoint;
}