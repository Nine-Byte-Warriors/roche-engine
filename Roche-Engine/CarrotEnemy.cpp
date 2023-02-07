#include "stdafx.h"
#include "CarrotEnemy.h"
#include "Health.h"

CarrotEnemy::CarrotEnemy(const std::shared_ptr<Health>& health, const std::shared_ptr<Collider>& collider)
{
	std::function<void(Collider&)> f = std::bind(&CarrotEnemy::Hit, this, std::placeholders::_1);
	collider->AddOnEnterCallback(f);
	m_health = health;
}

void CarrotEnemy::Update(float dt)
{
}

void CarrotEnemy::Hit(Collider& collider)
{
	if (collider.EntityType() == "Projectile")
	{
		m_health->TakeDamage(1000);
		OutputDebugStringA("PROJECTILE \n");
	}
}
