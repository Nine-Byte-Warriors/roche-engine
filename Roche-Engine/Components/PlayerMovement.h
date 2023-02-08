#pragma once
#ifndef PLAYERMOVEMENT_H
#define PLAYERMOVEMENT_H

#include "Physics.h"

class PlayerMovement : public Listener
{
public:
	PlayerMovement(const std::shared_ptr<Physics>& physics, const std::shared_ptr<Sprite>& sprite, float speed);
	~PlayerMovement();

	void Update(const float dt);
	void SetSpeed(float speed);
	void Dash();

	// Inherited via Listener
	virtual void HandleEvent(Event* event) override;
	void AddToEvent() noexcept;

private:
	std::shared_ptr<Physics> m_physics;
	std::shared_ptr<Sprite> m_sprite;
	float m_speed;
	float m_currentSpeed;
	float m_dashSpeed;
	bool m_bDash;
	bool m_bshouldMove;

};

#endif