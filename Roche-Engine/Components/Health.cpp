#include "stdafx.h"
#include "Health.h"
#include "AudioEngine.h"

#define PLAYER "Player"
#define ENEMY "EnemyCarrot"

void Health::SetHealth( float maxHealth )
{
	m_fMaxHealth = maxHealth;
	m_fCurrentHealth = m_fMaxHealth;
}

void Health::TakeDamage( float damageAmount )
{
	m_fCurrentHealth -= damageAmount;
	std::string currhealth = std::to_string(m_fCurrentHealth);
	OutputDebugStringA(currhealth.c_str());

	if ( m_fCurrentHealth <= 0 )
	{
		m_fCurrentHealth = 0;
		if (m_sType == "Player")
		{
			AudioEngine::GetInstance()->PlayAudio(PLAYER, "EntityDeath", SFX);
			EventSystem::Instance()->AddEvent(EVENTID::PlayerDeath, this);

		}
		else if (m_sType == "Enemy")
		{
			AudioEngine::GetInstance()->PlayAudio(ENEMY, "EntityDeath", SFX);
			EventSystem::Instance()->AddEvent(EVENTID::EnemyDeath, &m_iEntityNum);
		}
	}
}

void Health::Heal( float healAmount )
{
	m_fCurrentHealth += healAmount;
	if ( m_fCurrentHealth >= m_fMaxHealth )
		m_fCurrentHealth = m_fMaxHealth;
}

void Health::AddToEvent() noexcept
{
	EventSystem::Instance()->AddClient( EVENTID::PlayerDamage, this );
	EventSystem::Instance()->AddClient( EVENTID::PlayerHeal, this );
}

void Health::RemoveFromEvent() noexcept
{
	EventSystem::Instance()->RemoveClient( EVENTID::PlayerDamage, this );
	EventSystem::Instance()->RemoveClient( EVENTID::PlayerHeal, this );
}

void Health::HandleEvent( Event* event )
{
	switch ( event->GetEventID() )
	{
	case EVENTID::PlayerDamage:
		
		if ( m_sType == "Player" )
			TakeDamage( 1 );
		break;
	case EVENTID::PlayerHeal:
		if ( m_sType == "Player" )
			Heal( 1 );
		break;
	}
}