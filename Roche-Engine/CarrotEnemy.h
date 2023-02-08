#pragma once
#ifndef CARROTENEMY_H
#define CARROTENEMY_H

#include "Collider.h"

class Entity;

class CarrotEnemy
{
public:
	CarrotEnemy(Entity* entity);
	void Update(float dt);

private:
	Entity* m_entity;
	Entity* m_playerEntity;

	void Hit(Collider& collider);
};


#endif // !CARROTENEMY_H