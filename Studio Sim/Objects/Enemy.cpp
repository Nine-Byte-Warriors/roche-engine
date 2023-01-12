#include "stdafx.h"
#include "Enemy.h"
#include "Graphics.h"

Enemy::Enemy()
{
	m_sprite = std::make_shared<Sprite>();
	m_transform = std::make_shared<Transform>( m_sprite );
	m_physics = std::make_shared<Physics>( m_transform );
	m_agent = std::make_shared<Agent>( m_physics );
}

void Enemy::Initialize( Graphics& gfx, ConstantBuffer<Matrices>& mat, Sprite::Type type )
{
	m_sprite->Initialize( gfx.GetDevice(), gfx.GetContext(), type, mat );
}

void Enemy::Update( const float dt )
{
	m_sprite->Update( dt );
	m_agent->Update( dt );
	m_transform->Update();
}