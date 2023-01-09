#include "stdafx.h"
#include "Player.h"

Player::Player()
{
	m_sprite = std::make_shared<Sprite>();
	SetPosition( XMFLOAT3( 0.0f, 0.0f, 0.0f ) );
	SetRotation( XMFLOAT3( 0.0f, 0.0f, 0.0f ) );
	SetScale( XMFLOAT3( 1.0f, 1.0f, 1.0f ) );
	UpdateMatrix();
}

void Player::Update( const float dt )
{
	UpdateMatrix();
}

void Player::UpdateMatrix()
{
	worldMatrix = XMMatrixScaling( scale.x, scale.y, 1.0f ) *
		XMMatrixRotationRollPitchYaw( rotation.x, rotation.y, rotation.z ) *
		XMMatrixTranslation( position.x + scale.x / 2.0f, position.y + scale.y / 2.0f, position.z );
}