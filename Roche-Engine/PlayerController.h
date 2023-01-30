#pragma once

class Entity;

class PlayerController
{
public:
	PlayerController(Entity* entity);
	
	void Update(const float dt);
};