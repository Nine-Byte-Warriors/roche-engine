#include "stdafx.h"
#include "Enemy.h"

Enemy::Enemy()
{
	m_eType = ONION; // Default enemy type
	m_sprite = std::make_shared<Sprite>();
	m_transform = std::make_shared<Transform>();
	m_transform->SetPosition( XMFLOAT3( 0.0f, 0.0f, 0.0f ) );
	m_transform->SetRotation( XMFLOAT3( 0.0f, 0.0f, 0.0f ) );
	m_transform->SetScale( XMFLOAT3( 1.0f, 1.0f, 1.0f ) );
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
	case ONION:
		m_sprite->SetSSRows( 2 );
		m_sprite->SetSSColumns( 2 );
		return "Resources\\Textures\\onion_ss.png";
	case BEAN:
		m_sprite->SetSSRows( 2 );
		m_sprite->SetSSColumns( 1 );
		return "Resources\\Textures\\bean_ss.png";
	case CAULIFLOWER:
		m_sprite->SetSSRows( 1 );
		m_sprite->SetSSColumns( 1 );
		return "Resources\\Textures\\cauliflower_ss.png";
	case CARROT:
		m_sprite->SetSSRows( 2 );
		m_sprite->SetSSColumns( 2 );
		return "Resources\\Textures\\carrot_ss.png";
	case TOMATO:
		m_sprite->SetSSRows( 2 );
		m_sprite->SetSSColumns( 2 );
		return "Resources\\Textures\\tomato_ss.png";
	//case POTATO:
	//	m_sprite->SetSSRows( 2 );
	//	m_sprite->SetSSColumns( 2 );
	//	return "Resources\\Textures\\potato_ss.png";
	}
	return "";
}

void Enemy::UpdateMatrix()
{
	m_transform->SetWorldMatrix(
		XMMatrixScaling( m_transform->GetScaleFloat3().x, m_transform->GetScaleFloat3().y, 1.0f ) *
		XMMatrixRotationRollPitchYaw( m_transform->GetRotationFloat3().x, m_transform->GetRotationFloat3().y, m_transform->GetRotationFloat3().z ) *
		XMMatrixTranslation(
			m_transform->GetPositionFloat3().x + m_transform->GetScaleFloat3().x / 2.0f,
			m_transform->GetPositionFloat3().y + m_transform->GetScaleFloat3().y / 2.0f,
			m_transform->GetPositionFloat3().z
		)
	);
}