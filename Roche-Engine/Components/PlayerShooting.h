#pragma once
#ifndef PLAYER_SHOOTING_H
#define PLAYER_SHOOTING_H

#include "EventSystem.h"
#include "Emitter.h"

class PlayerShooting : public Listener
{
public:
	PlayerShooting(const std::shared_ptr<Emitter>& pEmitter) :
		m_pEmitter(pEmitter),
		m_bIsShooting(false)
	{ AddToEvent(); }
	~PlayerShooting() { RemoveFromEvent(); };

	inline void UpdateSpawnPosition(const Vector2f vPosition) noexcept { m_pEmitter->SetSpawnPosition(vPosition); }
	void Update(float dt);

	void AddToEvent() noexcept;
	void RemoveFromEvent() noexcept;
	void HandleEvent(Event* event) override;

private:
	std::shared_ptr<Emitter> m_pEmitter;
	bool m_bIsShooting;
};

#endif // !PLAYER_SHOOTING_H
