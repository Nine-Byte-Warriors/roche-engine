#include "stdafx.h"
#include "Health.h"
#include "Collider.h"

Health::Health(std::string type, int entityNum, std::shared_ptr<Collider> collider)
{
	m_sType = type;
	m_iEntityNum = entityNum;
	m_collider = collider;

	std::function<void(Collider&)> f = std::bind(&Health::Hit, this, std::placeholders::_1);
	collider->AddOnEnterCallback(f);

	AddToEvent();
}

void Health::SetHealth( float maxHealth )
{
	m_fMaxHealth = maxHealth;
	m_fCurrentHealth = m_fMaxHealth;
}

void Health::TakeDamage( float damageAmount )
{
	m_fCurrentHealth -= damageAmount;
	std::string currhealth = std::to_string(m_fCurrentHealth) + "\n";
	OutputDebugStringA(currhealth.c_str());

	if ( m_fCurrentHealth <= 0 )
	{
		m_fCurrentHealth = 0;
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

void Health::Hit(Collider& collider)
{
	if (collider.EntityType() == "PlayerProjectile" && m_sType == "Enemy")
		TakeDamage(1000);
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