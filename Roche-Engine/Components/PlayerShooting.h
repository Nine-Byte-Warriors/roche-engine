#pragma once
#ifndef PLAYER_SHOOTING_H
#define PLAYER_SHOOTING_H

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

private:
	void AddToEvent() noexcept;
	void RemoveFromEvent() noexcept;
	void HandleEvent(Event* event) override;

	std::shared_ptr<Emitter> m_pEmitter;
	Vector2f m_vMousePos;
	bool m_bIsShooting;
};

#endif // !PLAYER_SHOOTING_H
