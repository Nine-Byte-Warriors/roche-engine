#pragma once
#ifndef HEALTH_H
#define HEALTH_H

#include "EventSystem.h"


class Health : public Listener
{
private:
	float m_fEnemyCurrentHealth;
	float m_fPlayerCurrentHealth;
	float m_fPlayerMaxHealth;

public:
	
	Health();
	~Health();
	
	
	void PlayerDamage(float PlayerHealth, float DamageAmount);
	void EnemeyDamage(float EnemyHealth, float DamageAmount);
	void Heal(float HealAmount);

	void AddToEvent() noexcept;
	void HandleEvent(Event* event) override;

};

#endif