#pragma once

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