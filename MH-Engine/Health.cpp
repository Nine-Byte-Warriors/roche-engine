#include "stdafx.h"
#include "Health.h"


Health::Health()
{
	AddToEvent();
}



void Health::Initialize()
{
	m_fPlayerMaxHealth = 3;
	m_fEnemyMaxHealth = 3;
	m_fPlayerCurrentHealth = m_fPlayerMaxHealth;
	m_fEnemyCurrentHealth = m_fEnemyMaxHealth;
}



void Health::PlayerDamage(float DamageAmount)
{
	

	m_fPlayerCurrentHealth -= DamageAmount;

	if (m_fPlayerCurrentHealth <= 0)
	{ 
		OutputDebugStringA("dead");
	} //{ Lose Condition }
	else
		OutputDebugStringA("Alive");
}

void Health::EnemyDamage(float EnemyHealth, float DamageAmount)
{
	m_fEnemyCurrentHealth -= DamageAmount;

	if (m_fEnemyCurrentHealth <= 0)
	{
		OutputDebugStringA("dead");
		//{ kill enemy }
	} 
	else
		OutputDebugStringA("Alive");

}

void Health::Heal(float HealAmount)
{
	m_fPlayerCurrentHealth += HealAmount;

	if (m_fPlayerCurrentHealth >= m_fPlayerMaxHealth) m_fPlayerCurrentHealth = m_fPlayerMaxHealth;

}

void Health::AddToEvent() noexcept
{
	EventSystem::Instance()->AddClient(EVENTID::PlayerDamage, this);
	EventSystem::Instance()->AddClient(EVENTID::EnemeyDamage, this);
	EventSystem::Instance()->AddClient(EVENTID::PlayerHeal, this);
}

void Health::HandleEvent(Event* event)
{
	switch (event->GetEventID())
	{
	case EVENTID::PlayerDamage: PlayerDamage(1); break;
	case EVENTID::EnemeyDamage: EnemyDamage(3, 1); break;
	case EVENTID::PlayerHeal: Heal(1); break;
	default: break;
	}
}



