#pragma once
#ifndef ENEMY_H
#define ENEMY_H

class Graphics;
#include "Agent.h"
#include "Physics.h"

class Enemy
{
public:
	Enemy();
	void Initialize( Graphics& gfx, ConstantBuffer<Matrices>& mat, Sprite::Type type );
	void Update( const float dt );

	inline std::shared_ptr<Agent> GetAI() const noexcept { return m_agent; }
	inline std::shared_ptr<Sprite> GetSprite() const noexcept { return m_sprite; }
	inline std::shared_ptr<Physics> GetPhysics() const noexcept { return m_physics; }
	inline std::shared_ptr<Transform> GetTransform() const noexcept { return m_transform; }
private:
	std::shared_ptr<Agent> m_agent;
	std::shared_ptr<Sprite> m_sprite;
	std::shared_ptr<Physics> m_physics;
	std::shared_ptr<Transform> m_transform;

	Vector2f* m_vEnemyPos;
};

#endif