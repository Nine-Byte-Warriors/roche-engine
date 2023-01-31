#pragma once
#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

class Entity;
#include "PlayerMovement.h"

class PlayerController
{
public:
	PlayerController(Entity* entity);
	
	void Update(const float dt);

private:
	Entity* m_Entity;
	std::shared_ptr<PlayerMovement> m_playerMovement;
};

#endif 