#pragma once
#ifndef PHYSICS_H
#define PHYSICS_H

#include "Vector2f.h"
#include "Transform.h"

class Physics
{
public:
	Physics( const std::shared_ptr<Transform>& transform );

	// get/set forces
	inline Vector2f GetNetForce() const { return m_vNetForce; }
	inline void SetNetForce( Vector2f netForce ) noexcept { m_vNetForce = netForce; }
	inline void SetNetForce( float x, float y, float z ) noexcept { m_vNetForce = { x, y }; }

	inline Vector2f GetVelocity() const { return m_vVelocity; }
	inline void SetVelocity( Vector2f velocity ) noexcept { m_vVelocity = velocity; }
	inline void SetVelocity( float x, float y, float z ) noexcept { m_vVelocity = { x, y }; }

	inline Vector2f GetAcceleration() const { return m_vAcceleration; }
	inline void SetAcceleration( Vector2f acceleration ) noexcept { m_vAcceleration = acceleration; }
	inline void SetAcceleration( float x, float y, float z ) noexcept { m_vAcceleration = { x, y }; }

	// update constants
	inline float GetMass() const noexcept { return m_fMass; }
	inline void SetMass( float newMass ) noexcept { m_fMass = newMass; }
	inline float GetDragFactor() const noexcept { return m_fDragFactor; }
	inline float GetFriction() const noexcept { return m_fFrictionFactor; }

	// update forces
	virtual void Update( const float dt );
	virtual void ResetForces();
	
	inline void AddForce( Vector2f force ) noexcept { m_vNetForce += force; }
	inline std::shared_ptr<Transform> GetTransform() const noexcept { return m_transform; }
protected:
	// update forces
	void Friction( const float dt );
	void Velocity();
	void Acceleration();
	void Drag();
	void ComputePosition();

	float m_fMass;
	Vector2f m_vFriction;
	Vector2f m_vNetForce;
	Vector2f m_vVelocity;
	Vector2f m_vAcceleration;
	std::shared_ptr<Transform> m_transform;

	// local variables
	static constexpr float m_fDragFactor = 2.0f;
	static constexpr float m_fFrictionFactor = 0.1f;
};

#endif