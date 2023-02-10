#pragma once

#ifndef LEVELTRIGGER_H
#define LEVELTRIGGER_H

#include <functional>
#include "Collider.h"
#include "EventSystem.h"
#include "GameManager.h"

class LevelTrigger : public Listener
{
public:
	LevelTrigger(const std::shared_ptr<Collider> collider);
	~LevelTrigger();

	void PlayerInTrigger(Collider& collider);
	void PlayerOutTrigger(Collider& collider);
private:
	Phase m_currentGamePhase;
	bool m_bEventFired = false;
	virtual void HandleEvent(Event* event) override;
	void AddToEvent() noexcept;
};

#endif