#include "stdafx.h"
#include "Health.h"
#include "EventSystem.h"

void Health::SetHealth( float maxHealth )
{
	m_fMaxHealth = maxHealth;
	m_fCurrentHealth = m_fMaxHealth;
}

void Health::TakeDamage( float damageAmount )
{
	m_fCurrentHealth -= damageAmount;
	if ( m_fCurrentHealth <= 0 )
	{
		if ( m_sType == "Player" )
			EventSystem::Instance()->AddEvent( EVENTID::PlayerDeath, this );
		else if ( m_sType == "Enemy" )
			EventSystem::Instance()->AddEvent( EVENTID::EnemyDeath, &m_iEntityNum );
	}
}

void Health::Heal( float healAmount )
{
	m_fCurrentHealth += healAmount;
	if ( m_fCurrentHealth >= m_fMaxHealth )
		m_fCurrentHealth = m_fMaxHealth;
}