#include "stdafx.h"
#include "Health.h"


Health::Health()
{
	AddToEvent();
}

void Health::Initialize()
{
	//Temperay (for testing)
	m_fPlayerMaxHealth = 3;
	m_fEnemyMaxHealth = 3;
	m_fPlayerCurrentHealth = m_fPlayerMaxHealth;
	m_fEnemyCurrentHealth = m_fEnemyMaxHealth;
}


void Health::Damage(float EnityHealth, float DamageAmount)
{
	EnityHealth -= DamageAmount;
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
	case EVENTID::PlayerDamage: Damage(m_fPlayerCurrentHealth, 1); break;
	case EVENTID::EnemeyDamage: Damage(m_fEnemyMaxHealth, 1); break;
	case EVENTID::PlayerHeal: Heal(1); break;
	default: break;
	}
}



