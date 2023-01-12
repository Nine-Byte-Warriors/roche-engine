#pragma once
#ifndef ENEMY_H
#define ENEMY_H

#include "Agent.h"
#include "Sprite.h"
#include "Transform.h"

class Graphics;

class Enemy
{
public:
	Enemy();
	void Initialize( Graphics& gfx, ConstantBuffer<Matrices>& mat, Sprite::Type type );
	void Update( const float dt );

	inline std::shared_ptr<Agent> GetAI() const noexcept { return m_agent; }
	inline std::shared_ptr<Sprite> GetSprite() const noexcept { return m_sprite; }
	inline std::shared_ptr<Transform> GetTransform() const noexcept { return m_transform; }
private:
	std::shared_ptr<Agent> m_agent;
	std::shared_ptr<Sprite> m_sprite;
	std::shared_ptr<Transform> m_transform;
};

#endif