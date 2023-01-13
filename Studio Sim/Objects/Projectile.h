#pragma once
#ifndef PROJECTILE_H
#define PROJECTILE_H

class Graphics;
#include "Physics.h"
#include "EventSystem.h"
#include "Vector2f.h"

class Projectile : public Listener
{
public:
	Projectile();
	void Initialize(const Graphics& gfx, ConstantBuffer<Matrices>& mat, Sprite::Type type);
	void Update(const float dt);

	inline std::shared_ptr<Sprite> GetSprite() const noexcept { return m_sprite; }
	inline std::shared_ptr<Physics> GetPhysics() const noexcept { return m_physics; }
	inline std::shared_ptr<Transform> GetTransform() const noexcept { return m_transform; }
	
	void SpawnProjectile(Vector2f vSpawnPosition, Vector2f vTargetPosition, float fLifeTime);

	void AddToEvent() noexcept;
	void HandleEvent(Event* event) override;
	
private:
	std::shared_ptr<Sprite> m_sprite;
	std::shared_ptr<Physics> m_physics;
	std::shared_ptr<Transform> m_transform;
	
	float m_fLifeTime;
	Vector2f m_vSpawnPosition;
	Vector2f m_vTargetPosition;
};

#endif // !PROJECTILE_H