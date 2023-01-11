#pragma once
#ifndef PHYSICS_H
#define PHYSICS_H

#include "Vector2f.h"
#include "Transform.h"

class Physics
{
public:
	Physics( const std::shared_ptr<Transform>& transform );
	
	inline void Move( float x, float y ) noexcept { m_vNetForce = { x, y }; }
	inline std::shared_ptr<Transform> GetTransform() const noexcept { return m_transform; }

	// get/set forces
	inline Vector2f GetNetForce() const { return m_vNetForce; }
	inline Vector2f GetVelocity() const { return m_vVelocity; }
	inline Vector2f GetAcceleration() const { return m_vAcceleration; }

	inline void SetNetForce( Vector2f netForce ) noexcept { m_vNetForce = netForce; }
	inline void SetNetForce( float x, float y, float z ) noexcept { m_vNetForce = { x, y }; }
	inline void SetVelocity( Vector2f velocity ) noexcept { m_vVelocity = velocity; }
	inline void SetVelocity( float x, float y, float z ) noexcept { m_vVelocity = { x, y }; }
	inline void SetAcceleration( Vector2f acceleration ) noexcept { m_vAcceleration = acceleration; }
	inline void SetAcceleration( float x, float y, float z ) noexcept { m_vAcceleration = { x, y }; }

	// update constants
	inline float GetMass() const noexcept { return m_fMass; }
	inline float GetDragFactor() const noexcept { return m_fDrag; }
	inline float GetInverseMass() const noexcept { return m_fInverseMass; }
	inline float GetFriction() const noexcept { return m_fFrictionMultiplier; }

	inline void SetMass( float newMass ) noexcept { m_fMass = newMass; }
	inline void SetDragFactor( float dragFactor ) noexcept { m_fDrag = dragFactor; }
	inline void SetInverseMass( float value ) noexcept { m_fInverseMass = 1.0f / value; }
	inline void SetFriction( float friction ) noexcept { m_fFrictionMultiplier = friction; }

	// update forces
	virtual void Update( const float dt );
	void AddThrust( Vector2f force, float duration );
	inline void AddForce( Vector2f force ) noexcept { m_vNetForce += force; }
	virtual void ResetForces();
protected:
	float m_fMass;
	float m_fDrag;
	bool m_bUseLaminar;
	float m_fInverseMass;

	Vector2f m_vFriction;
	Vector2f m_vNetForce;
	Vector2f m_vVelocity;
	Vector2f m_vAcceleration;

	std::shared_ptr<Transform> m_transform;
private:
	// update forces
	void ApplyThrust( const float dt );

	void Acceleration();
	void Friction( const float dt );
	void Velocity( const float dt );

	void ComputePosition( const float dt );

	// local variables
	float m_fFrictionMultiplier = 0.0002f;
	std::vector<std::pair<Vector2f, float>> m_thrustForces;
};

#endif