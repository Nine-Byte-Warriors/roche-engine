#include "stdafx.h"
#include "EnemyController.h"

EnemyController::EnemyController(
	const std::shared_ptr <Physics>& pPhysics,
	const std::shared_ptr <Sprite>& pSprite,
	const std::shared_ptr <Emitter>& pEmitter
) : BaseController(pPhysics, pSprite, pEmitter)
{
	//m_vSpawnOffset = Vector2f(pSprite->GetWidth() / 2.0f, pSprite->GetHeight() / 2.0f);
}

void EnemyController::Update(const float dt)
{
	//m_playerMovement->Update(dt);
	//m_playerShooting->Update(dt);
}