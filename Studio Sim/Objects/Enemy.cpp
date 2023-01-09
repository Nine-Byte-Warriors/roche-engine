#include "stdafx.h"
#include "Enemy.h"

Enemy::Enemy()
{
	m_eType = ONION; // Default enemy type
	m_sprite = std::make_shared<Sprite>();
	SetPosition( XMFLOAT3( 0.0f, 0.0f, 0.0f ) );
	SetRotation( XMFLOAT3( 0.0f, 0.0f, 0.0f ) );
	SetScale( XMFLOAT3( 1.0f, 1.0f, 1.0f ) );
	UpdateMatrix();
}

void Enemy::Update( const float dt )
{
	UpdateMatrix();
}

std::string Enemy::GetTypePath( EnemyType type ) noexcept
{
	switch ( type )
	{
	case ONION: return "Resources\\Textures\\onion_ss.png";
	case BEAN: return "Resources\\Textures\\bean_ss.png";
	case CAULIFLOWER: return "Resources\\Textures\\cauliflower_ss.png";
	case CARROT: return "Resources\\Textures\\carrot_ss.png";
	case TOMATO: return "Resources\\Textures\\tomato_ss.png";
	case POTATO: return "Resources\\Textures\\potato_ss.png";
	}
	return "";
}

void Enemy::UpdateMatrix()
{
	worldMatrix = XMMatrixScaling( scale.x, scale.y, 1.0f ) *
		XMMatrixRotationRollPitchYaw( rotation.x, rotation.y, rotation.z ) *
		XMMatrixTranslation( position.x + scale.x / 2.0f, position.y + scale.y / 2.0f, position.z );
}