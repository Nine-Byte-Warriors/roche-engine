#pragma once

#ifndef LEVELTRIGGER_H
#define LEVELTRIGGER_H

#include <functional>
#include "Collider.h"
#include "EventSystem.h"

class LevelTrigger : public Listener
{
public:
	LevelTrigger(const std::shared_ptr<Collider> collider);
	~LevelTrigger();

	void PlayerInTrigger(Collider& collider);
private:
	
	bool m_bOnGameLevel = true;
	virtual void HandleEvent(Event* event) override;
	void AddToEvent() noexcept;
};

#endif