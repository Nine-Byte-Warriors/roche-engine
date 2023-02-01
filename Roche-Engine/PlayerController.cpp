#include "PlayerController.h"
#include "stdafx.h"
#include "Entity.h"

PlayerController::PlayerController(Entity* entity)
{
	m_Entity = entity;
	m_playerMovement = std::make_shared<PlayerMovement>(entity->GetPhysics(), 10);
}

void PlayerController::Update(const float dt)
{
	m_playerMovement->Update(dt);
}
 