#include "stdafx.h"
#include "PlayerController.h"

PlayerController::PlayerController(
	const std::shared_ptr <Physics>&	pPhysics,
	const std::shared_ptr <Sprite>&		pSprite,
	const std::shared_ptr <Emitter>&	pEmitter
) : BaseController(pPhysics, pSprite, pEmitter)
{
	m_playerMovement = std::make_shared<PlayerMovement>(pPhysics, pSprite, 10);
	m_playerShooting = std::make_shared<PlayerShooting>(pEmitter);

	m_vSpawnOffset = Vector2f(pSprite->GetWidth() / 2.0f, pSprite->GetHeight() / 2.0f);
}

void PlayerController::Update(const float dt)
{
	m_playerMovement->Update(dt);
	
	Vector2f vPosition = m_pPhysics->GetTransform()->GetPosition();
	Vector2f vOffSet = Vector2f(m_pSprite->GetWidth() / 2.0f, m_pSprite->GetHeight() / 2.0f);

	m_playerShooting->UpdateSpawnPosition(vPosition + vOffSet);
	m_playerShooting->Update(dt);
}
