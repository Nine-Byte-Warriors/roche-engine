#pragma once
#ifndef HEALTH_H
#define HEALTH_H

#include "EventSystem.h"


class Health 
{
public:
	float m_fEnemyCurrentHealth;
	float m_fPlayerCurrentHealth;

	float m_fPlayerMaxHealth = 3;
	float m_fEnemyMaxHealth = 3;

	Health();
	
	void Initialize();

	void PlayerDamage(float DamageAmount);
	void EnemyDamage(float DamageAmount);
	void Heal(float HealAmount);



};

#endif