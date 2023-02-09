#pragma once
#ifndef AGENT_H
#define AGENT_H

#include "Physics.h"
#include "AIStateMachine.h"

class Emitter;

namespace AILogic { class AIState; }

/// <summary>
/// Create a 2D agent object.
/// Inherits from GameObject2D.h to allow for modification of position/rotation/scale data in 2D space.
/// </summary>
class Agent : public Listener
{
public:
	Agent( const std::shared_ptr<Physics>& physics );
	~Agent() { RemoveFromEvent(); }
	void Update(const float dt);
#if _DEBUG
	void SpawnControlWindow(Vector2f fGO, Vector2f fTarg) noexcept;
#endif

	inline float GetSpeed() const noexcept { return m_fSpeed; }
	inline void SetSpeed(float speed) { m_fSpeed = speed; }
	inline Vector2f GetTargetPosition() const noexcept { return m_vTargetPos; }
	inline std::shared_ptr<Physics> GetPhysics() const noexcept { return m_physics; }

	void SetBehaviour(AILogic::AIStateTypes behaviour);
	void ResetBehaviour();

	void SetEmitter(std::shared_ptr<Emitter>& pEmitter) { m_pEmitter = pEmitter; }
	std::shared_ptr<Emitter> GetEmitter() const { return m_pEmitter; }
	inline bool IsStateMachineNULL() { return m_pStateMachine; }

	void AddToEvent() noexcept;
	void RemoveFromEvent() noexcept;
	void HandleEvent(Event* event) override;

private:
	std::map<AILogic::AIStateTypes, AILogic::AIState*> m_mapStates;
	AILogic::AIStateMachine* m_pStateMachine;
	std::shared_ptr<Physics> m_physics;
	bool m_bTargetMouse = true;
	Vector2f m_vTargetPos;
	float m_fSpeed;

	std::shared_ptr<Emitter> m_pEmitter;
};

#endif