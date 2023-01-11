#include "stdafx.h"
#include "Enemy.h"

Enemy::Enemy()
{
	m_sprite = std::make_shared<Sprite>();
	m_transform = std::make_shared<Transform>();
	
	m_eType = ONION; // Default enemy type
	m_transform->SetPosition( XMFLOAT3( 0.0f, 0.0f, 0.0f ) );
	m_transform->SetRotation( XMFLOAT3( 0.0f, 0.0f, 0.0f ) );
	m_transform->SetScale( XMFLOAT3( 1.0f, 1.0f, 1.0f ) );
	m_transform->Update(); // Initial updated required

	m_agent = std::make_shared<Agent>( m_transform );
}

void Enemy::Update( const float dt )
{
	m_sprite->Update( dt );
	m_agent->Update( dt );
	m_transform->Update();
}

std::string Enemy::GetTypePath( EnemyType type ) noexcept
{
	// Made it better for now, but still bad
	switch ( type )
	{
	case ONION:
		return "Resources\\Textures\\onion_ss.png";
	case BEAN:
		return "Resources\\Textures\\bean_ss.png";
	case CAULIFLOWER:
		return "Resources\\Textures\\cauliflower_ss.png";
	case CARROT:
		return "Resources\\Textures\\carrot_ss.png";
	case TOMATO:
		return "Resources\\Textures\\tomato_ss.png";
	}
	return "";
}