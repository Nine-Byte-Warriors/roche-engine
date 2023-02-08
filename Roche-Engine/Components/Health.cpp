#include "stdafx.h"
#include "Health.h"
#include "AudioEngine.h"
#include "Collider.h"

#define PLAYER "Player"
#define ENEMY "EnemyCarrot"


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
#if _DEBUG
	std::string currhealth = std::to_string(m_fCurrentHealth) + "\n";
	OutputDebugStringA(currhealth.c_str());
#endif

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
#if _DEBUG
			std::string outputdead = m_sType + " " + std::to_string(m_iEntityNum) + " Has Died! \n";
			OutputDebugStringA(outputdead.c_str());
#else
			EventSystem::Instance()->AddEvent( EVENTID::EnemyDeath, &m_iEntityNum );
#endif
			EventSystem::Instance()->AddEvent(EVENTID::UpdateScore);
			EventSystem::Instance()->AddEvent(EVENTID::GainCoins);
		}
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
	{
		AudioEngine::GetInstance()->PlayAudio(ENEMY, "EntityHit", SFX);
		TakeDamage(1);
	}

	if (collider.EntityType() == "EnemyProjectile" && m_sType == "Player")
	{
		AudioEngine::GetInstance()->PlayAudio(PLAYER, "EntityHit", SFX);
		TakeDamage(1);

	if (collider.EntityType() == "Enemy" && m_sType == "Player")
	{
		AudioEngine::GetInstance()->PlayAudio(PLAYER, "EntityHit", SFX);
		TakeDamage(1);
	}
}

void Health::SetEntityNum(int num)
{
	m_iEntityNum = num;
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