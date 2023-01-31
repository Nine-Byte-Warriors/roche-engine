#pragma once
#ifndef PLAYERMOVEMENT_H
#define PLAYERMOVEMENT_H

#include "Physics.h"
#include "Events/EventSystem.h"

class PlayerMovement : Listener
{
public:
	PlayerMovement(std::shared_ptr<Physics> physics, float speed);
	~PlayerMovement();

	void Update(const float dt);
	void SetSpeed(float speed);
	void Dash();

	// Inherited via Listener
	virtual void HandleEvent(Event* event) override;
	void AddToEvent() noexcept;

private:
	std::shared_ptr<Physics> m_physics;
	float m_speed;
	float m_currentSpeed;
	float m_dashSpeed;
	bool m_bDash;
	bool m_bshouldMove;

};

#endif