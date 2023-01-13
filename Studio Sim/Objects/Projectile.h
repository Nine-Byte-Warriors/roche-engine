#pragma once
#ifndef PROJECTILE_H
#define PROJECTILE_H

class Graphics;
#include "Physics.h"

class Projectile
{
public:
	Projectile(float fSpeed);
	void Initialize(const Graphics& gfx, ConstantBuffer<Matrices>& mat, Sprite::Type type);
	void Update(const float dt);
	void Draw(ID3D11DeviceContext* context, XMMATRIX orthoMatrix);

	inline std::shared_ptr<Sprite> GetSprite() const noexcept { return m_sprite; }
	inline std::shared_ptr<Physics> GetPhysics() const noexcept { return m_physics; }
	inline std::shared_ptr<Transform> GetTransform() const noexcept { return m_transform; }
	
	inline bool IsAlive() const noexcept { return m_fLifeTime > 0.0f; }
	
	void SpawnProjectile(Vector2f vSpawnPosition, Vector2f vTargetPosition, float fLifeTime);
	
private:
	std::shared_ptr<Sprite> m_sprite;
	std::shared_ptr<Physics> m_physics;
	std::shared_ptr<Transform> m_transform;
	
	float m_fSpeed;
	float m_fLifeTime;
	Vector2f m_vSpawnPosition;
	Vector2f m_vTargetPosition;
	Vector2f m_vDirection;
};

#endif // !PROJECTILE_H