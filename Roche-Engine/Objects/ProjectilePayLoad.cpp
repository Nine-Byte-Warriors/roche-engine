#include "stdafx.h"
#include "ProjectilePayLoad.h"

ProjectilePayLoad::ProjectilePayLoad(std::vector<std::shared_ptr<Projectile>> vecProjectiles, std::string sImagePath)
{
	m_vecProjectiles = vecProjectiles;
	m_sImagePath = sImagePath;
	bIsActive = false;
}

void ProjectilePayLoad::Initialise(
	const Graphics& gfx,
	ConstantBuffer<Matrices>& mat/*,
	Vector2f size*/
)
{
	for (std::shared_ptr<Projectile> pProjectile : m_vecProjectiles)
		pProjectile->Initialize(gfx, mat, m_sImagePath/*, size*/);
}