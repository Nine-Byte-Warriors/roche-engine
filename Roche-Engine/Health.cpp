#include "stdafx.h"
#include "Health.h"


Health::Health()
{
	AddToEvent();
}

Health::~Health()
{
	EventSystem::Instance()->RemoveClient(EVENTID::PlayerDamage, this);
	EventSystem::Instance()->RemoveClient(EVENTID::PlayerDeath, this);
	EventSystem::Instance()->RemoveClient(EVENTID::EnemeyDamage, this);
	EventSystem::Instance()->RemoveClient(EVENTID::EnemeyDeath, this);
	EventSystem::Instance()->RemoveClient(EVENTID::PlayerHeal, this);
}

void Health::PlayerDamage(float PlayerHealth, float DamageAmount)
{
	PlayerHealth -= DamageAmount;
	if (m_fEnemyCurrentHealth <= 0)
	{
		OutputDebugStringA("dead");
		EventSystem::Instance()->AddEvent(EVENTID::PlayerDeath, this);
	}
}

void Health::EnemeyDamage(float EnemyHealth, float DamageAmount)
{
	EnemyHealth -= DamageAmount;
	if (m_fEnemyCurrentHealth <= 0)
	{
		OutputDebugStringA("dead");
		EventSystem::Instance()->AddEvent(EVENTID::EnemeyDeath, this);
	}
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
	case EVENTID::PlayerDamage: 
		m_fPlayerCurrentHealth = *static_cast<float*>(event->GetData());
		PlayerDamage(m_fPlayerCurrentHealth, 1); break;
	case EVENTID::EnemeyDamage: 
		m_fEnemyCurrentHealth = *static_cast<float*>(event->GetData());
		EnemeyDamage(m_fEnemyCurrentHealth, 1); break;
	case EVENTID::PlayerHeal: Heal(1); break;
	default: break;
	}
}



