#pragma once
#ifndef ENEMY_H
#define ENEMY_H

#include "Sprite.h"
#include "GameObject.h"

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

class Enemy : public GameObject
{
public:
	Enemy();
	void Update( const float dt );

	std::string GetTypePath( EnemyType type ) noexcept;
	inline std::shared_ptr<Sprite> GetSprite() const noexcept { return m_sprite; }
private:
	void UpdateMatrix() override;
	EnemyType m_eType;
	std::shared_ptr<Sprite> m_sprite;
};

#endif