#pragma once
#ifndef PROJECTILE_H
#define PROJECTILE_H

class Graphics;
#include "Physics.h"

class Projectile
{
public:
	Projectile(float fSpeed, float fLifeTime = 999.9f);
	~Projectile() {}
	
	void Initialize(const Graphics& gfx, ConstantBuffer<Matrices>& mat, Sprite::Type type);
	void Initialize(const Graphics& gfx, ConstantBuffer<Matrices>& mat, std::string sSpritePath);
	void Update(const float dt);
	void Draw(ID3D11DeviceContext* context, XMMATRIX orthoMatrix);

	inline std::shared_ptr<Sprite> GetSprite() const noexcept { return m_sprite; }
	inline std::shared_ptr<Physics> GetPhysics() const noexcept { return m_physics; }
	inline std::shared_ptr<Transform> GetTransform() const noexcept { return m_transform; }
	
	inline void SetSpeed(const float fSpeed) noexcept { m_fSpeed = fSpeed; }
	inline void SetLifeTime(const float fLifeTime) noexcept { m_fLifeTime = fLifeTime; }
	inline void SetDirection(const Vector2f vDirection) noexcept { m_vDirection = vDirection; }
	inline void SetOffSet(const Vector2f vOffSet) noexcept { m_vOffSet = vOffSet; }
	inline void SetWave(const float vAngle, const float fAmp, const float fFreq) noexcept 
		{ m_bFixedDirection = false; m_fAngle = vAngle; m_fAmplitude = fAmp; m_fFrequency = fFreq; }
	inline bool IsAlive() const noexcept { return m_fLifeTime > 0.0f; }
	
	void SpawnProjectile(Vector2f vSpawnPosition, Vector2f vTargetPosition, float fLifeTime);
	void SpawnProjectile(Vector2f vSpawnPosition, float fLifeTime);
	
private:
	void CalcDirection();

	std::shared_ptr<Sprite> m_sprite;
	std::shared_ptr<Physics> m_physics;
	std::shared_ptr<Transform> m_transform;
	
	float m_fSpeed;
	float m_fLifeTime;
	float m_fMaxLifeTime;
	Vector2f m_vSpawnPosition;
	Vector2f m_vTargetPosition;
	Vector2f m_vDirection;
	Vector2f m_vOffSet;
	Vector2f m_vAnchorPosition;

	bool m_bFixedDirection;
	float m_fAngle;
	float m_fAmplitude;
	float m_fFrequency;
};

#endif // !PROJECTILE_H