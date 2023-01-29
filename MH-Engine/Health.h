#pragma once
#ifndef HEALTH_H
#define HEALTH_H

#include "EventSystem.h"


class Health : public Listener
{
public:
	float m_fEnemyCurrentHealth;
	float m_fPlayerCurrentHealth;

	float m_fPlayerMaxHealth;
	float m_fEnemyMaxHealth;



	Health();
	

	void Initialize();

	void PlayerDamage(float DamageAmount);
	void EnemyDamage(float EnemyHealth, float DamageAmount);
	void Heal(float HealAmount);

	void AddToEvent() noexcept;
	void HandleEvent(Event* event) override;

};

#endif