#pragma once
#ifndef PLAYER_H
#define PLAYER_H

class Graphics;
#include "Physics.h"
#include "EventSystem.h"
#include "ProjectileManager.h"

class Player : public Listener
{
public:
	Player();
	void Initialize( const Graphics& gfx, ConstantBuffer<Matrices>& mat );
	void Update( const float dt );
#if _DEBUG
	void SpawnControlWindow();
#endif

	void AddToEvent() noexcept;
	void HandleEvent( Event* event ) override;

	inline std::shared_ptr<Sprite> GetSprite() const noexcept { return m_sprite; }
	inline std::shared_ptr<Physics> GetPhysics() const noexcept { return m_physics; }
	inline std::shared_ptr<Transform> GetTransform() const noexcept { return m_transform; }
	inline std::shared_ptr<ProjectileManager> GetProjectileManager() const noexcept { return m_pProjectileManager; }
private:
	Vector2f* m_vPlayerPos;
	std::shared_ptr<Sprite> m_sprite;
	std::shared_ptr<Physics> m_physics;
	std::shared_ptr<Transform> m_transform;
	std::shared_ptr<ProjectileManager> m_pProjectileManager;
};

#endif