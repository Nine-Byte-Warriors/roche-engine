#pragma once
#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include "PlayerMovement.h"
#include "PlayerShooting.h"
#include "Emitter.h"
#include "Physics.h"
#include "Sprite.h"

class PlayerController
{
public:
	PlayerController(
		const std::shared_ptr <Physics>& pPhysics,
		const std::shared_ptr <Sprite>& m_pSprite,
		const std::shared_ptr <Emitter>& pEmitter
	);
	
	void Update(const float dt);

private:
	std::shared_ptr <Physics> m_pPhysics;
	std::shared_ptr <Sprite> m_pSprite;
	std::shared_ptr <Emitter> pEmitter;
	
	std::shared_ptr<PlayerMovement> m_playerMovement;
	std::shared_ptr<PlayerShooting> m_playerShooting;

	Vector2f m_vSpawnOffset;
};

#endif 