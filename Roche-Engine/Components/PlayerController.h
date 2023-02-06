#pragma once
#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include "BaseController.h"
#include "PlayerMovement.h"
#include "PlayerShooting.h"

class PlayerController : public BaseController
{
public:
	PlayerController(
		const std::shared_ptr <Physics>& pPhysics,
		const std::shared_ptr <Sprite>& pSprite,
		const std::shared_ptr <Emitter>& pEmitter
	);

	//virtual void Initialise() override;
	virtual void Update( const float dt ) override;

private:
	std::shared_ptr<PlayerMovement>			m_playerMovement;
	std::shared_ptr<PlayerShooting>			m_playerShooting;

	Vector2f								m_vSpawnOffset;
};

#endif // PLAYERCONTROLLER_H