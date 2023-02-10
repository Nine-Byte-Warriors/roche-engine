#pragma once
#ifndef PROJECTILEPAYLOAD_H
#define PROJECTILEPAYLOAD_H

#include "Projectile.h"

class ProjectilePayLoad
{
public:
	ProjectilePayLoad(std::vector<std::shared_ptr<Projectile>> vecProjectiles, std::string sImagePath);
	
	void Update(const float dt)
	{
		for (std::shared_ptr<Projectile> pProjectile : m_vecProjectiles)
			pProjectile->Update(dt);
	}

	void Initialise(const Graphics& gfx, ConstantBuffer<Matrices>& mat);
	
	void Draw(ID3D11DeviceContext* context, XMMATRIX orthoMatrix)
	{
		for (std::shared_ptr<Projectile> pProjectile : m_vecProjectiles)
			pProjectile->Draw(context, orthoMatrix);
	}

	void SetTargetPosition(Vector2f vTargetPosition)
	{
		m_vTargetPosition = vTargetPosition - (m_vecProjectiles[0]->GetSprite()->GetWidthHeight() / 2);
	}

	void SetSize(const Vector2f vSize)
	{
		for (std::shared_ptr<Projectile> pProjectile : m_vecProjectiles)
			pProjectile->GetSprite()->SetWidthHeight(vSize.x, vSize.y);
	}

	void SetMaxFrame(const Vector2f vMaxFrame)
	{
		for (std::shared_ptr<Projectile> pProjectile : m_vecProjectiles)
			pProjectile->GetSprite()->SetMaxFrame(vMaxFrame.x, vMaxFrame.y);
	}

	void SetMass(const float fMass)
	{
		for (std::shared_ptr<Projectile> pProjectile : m_vecProjectiles)
			pProjectile->GetPhysics()->SetMass(fMass);
	}

	void Fire(Vector2f vSpawnPosition, float fLifeTime = -1.0f)
	{
		bIsActive = true;

		for (std::shared_ptr<Projectile> pProjectile : m_vecProjectiles)
			pProjectile->SpawnProjectile(vSpawnPosition, fLifeTime);
	}

	void Fire(Vector2f vSpawnPosition, Vector2f vTargetPosition, float fLifeTime = -1.0f)
	{
		bIsActive = true;

		for (std::shared_ptr<Projectile> pProjectile : m_vecProjectiles)
			pProjectile->SpawnProjectile(vSpawnPosition, vTargetPosition, fLifeTime);
	}

	std::vector<std::shared_ptr<Projectile>> m_vecProjectiles;
	std::string m_sImagePath;
	bool bIsActive;
	Vector2f m_vTargetPosition;
};

#endif // !PROJECTILEPAYLOAD_H