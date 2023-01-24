#include "stdafx.h"
#include "Health.h"


void Health::Initialize()
{
	m_fPlayerMaxHealth = 3;
	m_fEnemyMaxHealth = 3;

	m_fPlayerCurrentHealth = m_fPlayerMaxHealth;
	m_fEnemyCurrentHealth = m_fEnemyMaxHealth;
	
	
}



void Health::PlayerDamage(float DamageAmount)
{
	m_fPlayerCurrentHealth - DamageAmount;

	if (m_fPlayerCurrentHealth <= 0) OutputDebugStringA("dead"); //{ Lose Condition }
	
	
	
}

void Health::EnemyDamage(float DamageAmount)
{
	m_fEnemyCurrentHealth - DamageAmount;

	if (m_fEnemyCurrentHealth <= 0); //{ kill enemy }

}

void Health::Heal(float HealAmount)
{
	m_fPlayerCurrentHealth - HealAmount;

	if (m_fPlayerCurrentHealth >= m_fPlayerMaxHealth) m_fPlayerCurrentHealth = m_fPlayerMaxHealth;

}



