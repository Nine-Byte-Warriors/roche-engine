#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "Sprite.h"
#include "GameObject.h"

class Player : public GameObject
{
public:
	Player();
	void Update( const float dt );
	inline std::shared_ptr<Sprite> GetSprite() const noexcept { return m_sprite; }
private:
	void UpdateMatrix() override;
	std::shared_ptr<Sprite> m_sprite;
};

#endif