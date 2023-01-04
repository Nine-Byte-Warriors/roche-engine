#pragma once
class AI
{
public:
	enum class Behaviour
	{
		Idle,
		Seek,
		Flee,
		Arrive,
		Pursue,
		Evade,
		Wander,
		ObstacleAvoidance,
		WallAvoidance,
		FollowPath,
		Patrol,
		Interpose,
		Hide,
		Flock,
		OffsetPursue,
		Seperation,
		Alignment,
		Cohesion,
		FollowLeader,
		NumBehaviours
	};
	
	AI();
	~AI();
	
	void Update(float dt)
	{
		// Update the current behaviour
//		(this->*behaviours[currentBehaviour])(dt);
	}
	
	void SetBehaviour(Behaviour behaviour);
	
	//void SetTarget(const Vector3& target);
	//
	//void SetTarget(Agent* target);
	//
	//void SetTarget(Agent* target, const Vector3& offset);
	//
	//void SetPath(Path* path) {
	//	
	//}
	
	void SetWanderOffset(float offset);
	
	void SetWanderRadius(float radius);
	
	void SetWanderRate(float rate) {
		m_wanderRate = rate;
	}
	
	//void CalculatePath(Agent* target) {
	//	
	//}
private:
	// Behaviours
	void Idle(float dt);
	void Seek(float dt);
	void Flee(float dt);
	void Arrive(float dt);
	void Pursue(float dt);
	void Evade(float dt);
	void Wander(float dt);
	void ObstacleAvoidance(float dt);
	void WallAvoidance(float dt);
	void FollowPath(float dt);
	void Patrol(float dt);
	void Interpose(float dt);
	void Hide(float dt);
	void Flock(float dt);
	void OffsetPursue(float dt);
	void Seperation(float dt);
	void Alignment(float dt);
	void Cohesion(float dt);
	void FollowLeader(float dt);

	// Pointer to the current behaviour
	void (AI::* behaviours[(int)Behaviour::NumBehaviours])(float);

	// Current behaviour
	Behaviour currentBehaviour;

	//// Target
	//Vector3 target;

	//// Target agent
	//Agent* targetAgent;

	//// Offset from target agent
	//Vector3 targetOffset;

	//// Path
	//Path* path;

	// Wander
	float m_wanderOffset;
	float m_wanderRadius;
	float m_wanderRate;
	float m_wanderAngle;

	// Flock
	float m_separationWeight;
	float m_alignmentWeight;
	float m_cohesionWeight;

	// Follow leader
	float m_leaderWeight;
};
