#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "Sprite.h"
#include "Physics.h"
#include "Transform.h"
#include "EventSystem.h"

class Player : public Listener
{
public:
	Player();
	void Update( const float dt );
	void SpawnControlWindow();

	void AddToEvent();
	void HandleEvent( Event* event ) override;

	inline std::shared_ptr<Sprite> GetSprite() const noexcept { return m_sprite; }
	inline std::shared_ptr<Physics> GetPhysics() const noexcept { return m_physics; }
	inline std::shared_ptr<Transform> GetTransform() const noexcept { return m_transform; }
private:
	std::shared_ptr<Sprite> m_sprite;
	std::shared_ptr<Physics> m_physics;
	std::shared_ptr<Transform> m_transform;
};

#endif