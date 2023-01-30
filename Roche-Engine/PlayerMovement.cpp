#include "stdafx.h"
#include "PlayerMovement.h"

PlayerMovement::PlayerMovement(Physics* physics, float speed)
{
	m_physics = physics;
	m_speed = speed;
	m_currentSpeed = m_speed;
	m_bDash = false;

	AddToEvent();
}

void PlayerMovement::Move(Vector2f direction)
{
	if (m_bDash)
	{
		m_physics->AddForce(direction * m_speed * 20);
		m_bDash = false;
		return;
	}

	m_physics->AddForce(direction * m_speed);
}

void PlayerMovement::SetSpeed(float speed)
{
	m_speed = speed;
}

void PlayerMovement::Dash()
{
	m_bDash = true;
}

void PlayerMovement::HandleEvent(Event* event)
{
	switch (event->GetEventID())
	{
	case EVENTID::PlayerUp: Move(Vector2f::Down());
		break;
	case EVENTID::PlayerLeft: Move(Vector2f::Left());
		break;
	case EVENTID::PlayerDown: Move(Vector2f::Up());
		break;
	case EVENTID::PlayerRight: Move(Vector2f::Right());
		break;
	case EVENTID::PlayerDash: Dash();
		break;
	default: break;
	}
}

void PlayerMovement::AddToEvent() noexcept
{
	EventSystem::Instance()->AddClient(EVENTID::PlayerUp, this);
	EventSystem::Instance()->AddClient(EVENTID::PlayerLeft, this);
	EventSystem::Instance()->AddClient(EVENTID::PlayerDown, this);
	EventSystem::Instance()->AddClient(EVENTID::PlayerRight, this);
	EventSystem::Instance()->AddClient(EVENTID::PlayerDash, this);
}
