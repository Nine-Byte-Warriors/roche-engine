#include "stdafx.h"
#include "Player.h"

Player::Player()
{
	m_sprite = std::make_shared<Sprite>();
	m_transform = std::make_shared<Transform>();
	m_transform->SetPosition( XMFLOAT3( 0.0f, 0.0f, 0.0f ) );
	m_transform->SetRotation( XMFLOAT3( 0.0f, 0.0f, 0.0f ) );
	m_transform->SetScale( XMFLOAT3( 1.0f, 1.0f, 1.0f ) );
	UpdateMatrix();
}

void Player::Update( const float dt )
{
	m_sprite->Update( dt );
	UpdateMatrix();
}

void Player::UpdateMatrix()
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