#include "stdafx.h"
#include "Physics.h"

Physics::Physics(const std::shared_ptr<Transform>& transform) : m_transform(transform)
{
	m_fMass = 20.0f;
	m_vFriction = { 0.0f, 0.0f };
	m_vNetForce = { 0.0f, 0.0f };
	m_vVelocity = { 0.0f, 0.0f };
	m_vAcceleration = { 0.0f, 0.0f };
}

void Physics::Update(const float dt)
{
	Friction(dt);
	Velocity();
	Acceleration();
	Drag();
	ComputePosition();
	m_vNetForce = { 0.0f, 0.0f };
}

void Physics::Friction( const float dt )
{
	// f = u * N
	Vector2f invVelocity = { -m_vVelocity.x, -m_vVelocity.y };
	if (m_vVelocity.Magnitude() < m_fFrictionFactor)
		m_vFriction = invVelocity / dt;
	else
		m_vFriction = invVelocity.Normalised() * m_fFrictionFactor;
}

void Physics::Velocity()
{
	// v = u + at
	m_vVelocity += m_vAcceleration;

	// apply friction
	m_vVelocity.x += m_fFrictionFactor * -( m_vVelocity.x );
	m_vVelocity.y += m_fFrictionFactor * -( m_vVelocity.y );
}

void Physics::Acceleration()
{
	// f = m * a   ->   a = f / m
	m_vAcceleration = m_vNetForce / m_fMass;
}

void Physics::Drag()
{
	m_vNetForce.x -= m_fDragFactor * m_vVelocity.x;
	m_vNetForce.y -= m_fDragFactor * m_vVelocity.y;
}

void Physics::ComputePosition()
{
	Vector2f position = m_transform->GetPosition();
	position.x += m_vVelocity.x + 0.5f * m_vAcceleration.x;
	position.y += m_vVelocity.y + 0.5f * m_vAcceleration.y;
	m_vVelocity += m_vAcceleration;
	m_transform->SetPosition(position);
}

void Physics::ResetForces()
{
	m_vNetForce = { 0.0f, 0.0f };
	m_vVelocity = { 0.0f, 0.0f };
	m_vAcceleration = { 0.0f, 0.0f };
}