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
	void Initialize( const std::shared_ptr<Transform>& transform );
	void Update(float dt);
	
	inline void SetVelocity(const Vector2f vel) { m_vVelocity = vel; }
	inline float GetSpeed() const noexcept { return m_fSpeed; }
	
	inline Vector2f GetPositionVector2f() const noexcept
	{
		XMFLOAT3 pos = m_transform->GetPositionFloat3();
		return Vector2f(pos.x, pos.y);
	}
	inline XMFLOAT3 GetTargetPos() const noexcept { return XMFLOAT3(m_vTargetPos.x, m_vTargetPos.y, 0.0f); }
	inline std::shared_ptr<Transform> GetTransform() const noexcept { return m_transform; }
	inline Vector2f GetTargetVector2f() const noexcept { return m_vTargetPos; }
	
	inline void AddToEvent() { EventSystem::Instance()->AddClient(EVENTID::PlayerPosition, this); }
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