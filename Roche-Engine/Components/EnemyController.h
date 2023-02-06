#pragma once
#ifndef ENEMYCONTROLLER_H
#define ENEMYCONTROLLER_H

#include "BaseController.h"

class EnemyController : public BaseController
{
public:
	EnemyController(
		const std::shared_ptr <Physics>& pPhysics,
		const std::shared_ptr <Sprite>& pSprite,
		const std::shared_ptr <Emitter>& pEmitter
	);

	//virtual void Initialise() override;
	virtual void Update(const float dt) override;

private:
	Vector2f								m_vSpawnOffset;
};

#endif // ENEMYCONTROLLER_H