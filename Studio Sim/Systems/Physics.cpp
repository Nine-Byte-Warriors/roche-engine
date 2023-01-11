#include "stdafx.h"
#include "Physics.h"

Physics::Physics(const std::shared_ptr<Transform>& transform) : m_transform(transform)
{
	m_fMass = 50.0f;
	m_bUseLaminar = true;

	m_fDrag = 2.0f;
	m_vFriction = { 0.0f, 0.0f };
	m_vNetForce = { 0.0f, 0.0f };
	m_vVelocity = { 0.0f, 0.0f };
	m_vAcceleration = { 0.0f, 0.0f };
}

void Physics::Update(const float dt)
{
	Weight();
	ApplyThrust(dt);
	Acceleration();
	Friction(dt);
	Velocity(dt);
	DragForce(dt);
	ComputePosition(dt);
	m_vNetForce = { 0.0f, 0.0f };
}

void Physics::Weight()
{
	// f = m * g
	m_fWeight = m_fMass * m_fGravity;
	m_vNetForce.y -= m_fWeight * m_fLimiter;
}

void Physics::ApplyThrust(const float dt)
{
	std::vector<int> deadForces;

	// update thrust forces
	for (int i = 0; i < m_thrustForces.size(); ++i)
	{
		m_vNetForce += m_thrustForces[i].first;
		m_thrustForces[i].second -= dt;
		if (m_thrustForces[i].second <= 0.0f)
			deadForces.push_back(i);
	}

	// remove expired forces
	for (int i = deadForces.size() - 1; i >= 0; --i)
		m_thrustForces.erase(m_thrustForces.begin() + deadForces[i]);
}

void Physics::AddThrust(Vector2f force, float duration)
{
	m_thrustForces.push_back(std::make_pair(force, duration));
}

void Physics::DragForce(const float dt)
{
	// calculate drag factor
	m_bUseLaminar ? DragLaminar() : DragTurbulent();

	// add to velocity and adjust for negative values
	if (m_vNetForce.x < 0.0f) // x values
		m_vVelocity.x *= 1.0f + m_vNetForce.x;
	else
		m_vVelocity.x *= 1.0f - m_vNetForce.x;

	if (m_vNetForce.y < 0.0f) // y values
		m_vVelocity.y *= 1.0f + m_vNetForce.y;
	else
		m_vVelocity.y *= 1.0f - m_vNetForce.y;
}

void Physics::DragLaminar()
{
	m_vNetForce -= m_vVelocity * m_fDrag;
}

void Physics::DragTurbulent()
{
	// magnitude of drag force
	float velocityMagnitude = m_vVelocity.Magnitude();
	Vector2f unitVelocity = m_vVelocity.Normalised();

	// calculate new drag factor
	float dragMagnitude = m_fDrag * velocityMagnitude * velocityMagnitude;
	Vector2f newVelocity = unitVelocity * dragMagnitude;

	// adjust for negative values
	if (newVelocity.x < 0.0f) // x values
		m_vNetForce.x += newVelocity.x;
	else
		m_vNetForce.x -= newVelocity.x;

	if (newVelocity.y < 0.0f) // y values
		m_vNetForce.y += newVelocity.y;
	else
		m_vNetForce.y -= newVelocity.y;
}

void Physics::Acceleration()
{
	// f = m * a   ->   a = f / m
	m_vAcceleration = m_vNetForce / m_fMass;
}

void Physics::Friction(const float dt)
{
	// f = u * N
	Vector2f invVelocity = { -m_vVelocity.x, -m_vVelocity.y };
	if (m_vVelocity.Magnitude() < m_fFrictionMultiplier * dt)
		m_vFriction = (invVelocity / dt);
	else
		m_vFriction = invVelocity.Normalised() * m_fFrictionMultiplier;
}

void Physics::Velocity(const float dt)
{
	// v = u + at
	m_vVelocity += m_vAcceleration * dt;

	// adjust for negative values
	if (m_vVelocity.x > 0.0f) // x values
		m_vVelocity.x -= m_fFrictionMultiplier;
	else if (m_vVelocity.x < 0.0f)
		m_vVelocity.x += m_fFrictionMultiplier;

	if (m_vVelocity.y > 0.0f) // y values
		m_vVelocity.y -= m_fFrictionMultiplier;
	else if (m_vVelocity.y < 0.0f)
		m_vVelocity.y += m_fFrictionMultiplier;
}

void Physics::ComputePosition(const float dt)
{
	Vector2f position = m_transform->GetPositionVector2f();
	position.x += m_vVelocity.x * dt + 0.5f * m_vAcceleration.x * dt * dt;
	position.y += m_vVelocity.y * dt + 0.5f * m_vAcceleration.y * dt * dt;
	m_vVelocity += m_vAcceleration * dt;
	m_transform->SetPosition(position);
}

void Physics::ResetForces()
{
	m_vNetForce = { 0.0f, 0.0f };
}