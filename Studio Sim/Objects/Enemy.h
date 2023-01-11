#pragma once
#ifndef ENEMY_H
#define ENEMY_H

#include "Agent.h"
#include "Sprite.h"
#include "Transform.h"

enum EnemyType
{
	ONION,
	BEAN,
	CAULIFLOWER,
	CARROT,
	TOMATO,
	POTATO,
	SIZEOFENEMYTYPE // Size of enum
};

class Enemy
{
public:
	Enemy();
	void Update( const float dt );

	std::string GetTypePath( EnemyType type ) noexcept;
	inline std::shared_ptr<Agent> GetAI() const noexcept { return m_agent; }
	inline std::shared_ptr<Sprite> GetSprite() const noexcept { return m_sprite; }
	inline std::shared_ptr<Transform> GetTransform() const noexcept { return m_transform; }
private:
	EnemyType m_eType;
	std::shared_ptr<Agent> m_agent;
	std::shared_ptr<Sprite> m_sprite;
	std::shared_ptr<Transform> m_transform;
};

#endif