#include "stdafx.h"
#include "CarrotEnemy.h"
#include "Entity.h"

CarrotEnemy::CarrotEnemy(Entity* entity)
{
	std::function<void(Collider&)> f = std::bind(&CarrotEnemy::Hit, this, std::placeholders::_1);
	entity->GetCollider()->AddOnEnterCallback(f);
	m_entity = entity;
}

void CarrotEnemy::Update(float dt)
{
}

void CarrotEnemy::Hit(Collider& collider)
{
	if (collider.EntityType() == "Projectile")
		m_entity->GetHealth()->TakeDamage(20);

	if (collider.EntityType() == "Projectile")
		OutputDebugStringA("PROJECTILE \n");
}
