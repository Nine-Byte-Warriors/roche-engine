#pragma once
#ifndef AGENT_H
#define AGENT_H

#include "Physics.h"
#include "EventSystem.h"
#include "AIStateMachine.h"

//namespace AILogic { class AIState; }

/// <summary>
/// Create a 2D agent object.
/// Inherits from GameObject2D.h to allow for modification of position/rotation/scale data in 2D space.
/// </summary>
class Agent : public Listener
{
public:
	Agent( const std::shared_ptr<Physics>& physics );
	void Update(const float dt);
	void SpawnControlWindow(Vector2f fGO, Vector2f fTarg) const noexcept;
	
	inline float GetSpeed() const noexcept { return m_fSpeed; }
	inline Vector2f GetTargetPosition() const noexcept { return m_vTargetPos; }
	inline std::shared_ptr<Physics> GetPhysics() const noexcept { return m_physics; }
	
	void AddToEvent() noexcept;
	void HandleEvent(Event* event) override;
private:
	/*AILogic::*/AIStateMachine* m_pStateMachine;
	std::vector</*AILogic::*/AIState*> m_vecStates;
	std::shared_ptr<Physics> m_physics;
	Vector2f m_vTargetPos;
	float m_fSpeed;
};

#endif