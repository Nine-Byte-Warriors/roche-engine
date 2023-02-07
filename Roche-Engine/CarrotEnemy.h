#pragma once
#ifndef CARROTENEMY_H
#define CARROTENEMY_H
#include <functional>
#include "Collider.h"

class Health;

class CarrotEnemy
{
public: 
	CarrotEnemy(const std::shared_ptr<Health>& health, const std::shared_ptr<Collider>& collider);
	void Update(float dt);

private:
	std::shared_ptr<Health> m_health;

	void Hit(Collider& collider);
};


#endif // !CARROTENEMY_H