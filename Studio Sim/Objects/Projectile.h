#pragma once
#ifndef PROJECTILE_H
#define PROJECTILE_H

class Graphics;
#include "Physics.h"

class Projectile
{
public:
	Projectile();
	void Initialize(const Graphics& gfx, ConstantBuffer<Matrices>& mat, Sprite::Type type);
	void Update(const float dt);

	inline std::shared_ptr<Sprite> GetSprite() const noexcept { return m_sprite; }
	inline std::shared_ptr<Physics> GetPhysics() const noexcept { return m_physics; }
	inline std::shared_ptr<Transform> GetTransform() const noexcept { return m_transform; }
	
private:
	std::shared_ptr<Sprite> m_sprite;
	std::shared_ptr<Physics> m_physics;
	std::shared_ptr<Transform> m_transform;
	
	float m_fLifeTime;
	Vector2f m_vSpawnPosition;
	Vector2f m_vTargetPosition;
};

#endif // !PROJECTILE_H