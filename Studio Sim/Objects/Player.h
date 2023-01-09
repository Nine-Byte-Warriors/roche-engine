#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "Sprite.h"
#include "Transform.h"

class Player
{
public:
	Player();
	void Update( const float dt );
	inline std::shared_ptr<Sprite> GetSprite() const noexcept { return m_sprite; }
	inline std::shared_ptr<Transform> GetTransform() const noexcept { return m_transform; }
private:
	void UpdateMatrix();
	std::shared_ptr<Sprite> m_sprite;
	std::shared_ptr<Transform> m_transform;
};

#endif