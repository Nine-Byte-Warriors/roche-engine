#pragma once
#include "Physics.h"
#include "Events/EventSystem.h"

class PlayerMovement : Listener
{
public:
	PlayerMovement(Physics* physics, float speed);

	void Move(Vector2f direction);
	void SetSpeed(float speed);
	void Dash();

	// Inherited via Listener
	virtual void HandleEvent(Event* event) override;
	void AddToEvent() noexcept;

private:
	Physics* m_physics;
	float m_speed;
	float m_currentSpeed;
	float m_dashSpeed;
	bool m_bDash;


};