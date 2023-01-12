#pragma once
#ifndef AGENT_H
#define AGENT_H

#include "Vector2f.h"
#include "Transform.h"
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
	Agent( const std::shared_ptr<Transform>& transform );
	void Update(float dt);
	void SpawnControlWindow(Vector2f fGO, Vector2f fTarg) const noexcept;
	
	inline void SetVelocity(const Vector2f vel) { m_vVelocity = vel; }
	inline float GetSpeed() const noexcept { return m_fSpeed; }
	
	inline Vector2f GetTargetPos() const noexcept { return Vector2f(m_vTargetPos.x, m_vTargetPos.y); }
	inline std::shared_ptr<Transform> GetTransform() const noexcept { return m_transform; }
	inline Vector2f GetTargetVector2f() const noexcept { return m_vTargetPos; }
	
	void AddToEvent() noexcept;
	void HandleEvent(Event* event) override;
private:
	/*AILogic::*/AIStateMachine* m_pStateMachine;
	std::vector</*AILogic::*/AIState*> m_vecStates;
	std::shared_ptr<Transform> m_transform;
	Vector2f m_vVelocity;
	Vector2f m_vTargetPos;
	float m_fSpeed;
};

#endif