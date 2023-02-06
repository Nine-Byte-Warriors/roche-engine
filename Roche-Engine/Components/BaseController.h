#pragma once
#ifndef BASECONTROLLER_H
#define BASECONTROLLER_H

#include "Emitter.h"
#include "Physics.h"
#include "Sprite.h"

class BaseController
{
public:
	BaseController(
		const std::shared_ptr <Physics>&	pPhysics,
		const std::shared_ptr <Sprite>&		pSprite,
		const std::shared_ptr <Emitter>&	pEmitter
	) : m_pPhysics(pPhysics), m_pSprite(pSprite), m_pEmitter(pEmitter) {};

	//virtual void Initialise() = 0;
	virtual void Update(const float dt) = 0;

protected:
	std::shared_ptr <Physics>				m_pPhysics;
	std::shared_ptr <Sprite>				m_pSprite;
	std::shared_ptr <Emitter>				m_pEmitter;
	
private:
	
};

#endif // BASECONTROLLER_H
