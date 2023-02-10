#include "stdafx.h"
#include "ProjectilePayLoad.h"

void ProjectilePayLoad::Initialise(std::vector<std::shared_ptr<Projectile>> vecProjectiles)
{
	m_vecProjectiles = vecProjectiles;
	bIsActive = false;
}