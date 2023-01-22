#include "stdafx.h"
#include "Enemy.h"
#include "Graphics.h"

Enemy::Enemy()
{
	m_vEnemyPos = new Vector2f();
	m_sprite = std::make_shared<Sprite>();
	m_transform = std::make_shared<Transform>( m_sprite );
	m_physics = std::make_shared<Physics>( m_transform );
	m_agent = std::make_shared<Agent>( m_physics );
	m_collider = std::make_shared<CircleCollider>( m_transform, 1, 1, 32 );
}

void Enemy::Initialize( Graphics& gfx, ConstantBuffer<Matrices>& mat, Sprite::Type type )
{
	m_sprite->Initialize( gfx.GetDevice(), gfx.GetContext(), type, mat );
	m_transform->SetPositionInit(gfx.GetWidth() * 0.4f, gfx.GetHeight() / 2);
	m_transform->SetScaleInit(m_sprite->GetWidth(), m_sprite->GetHeight());
}

void Enemy::Update( const float dt )
{
	m_sprite->Update( dt );
	m_agent->Update( dt );
	m_transform->Update();

	m_vEnemyPos->x = m_transform->GetPosition().x;
	m_vEnemyPos->y = m_transform->GetPosition().y;
	EventSystem::Instance()->AddEvent( EVENTID::TargetPosition, m_vEnemyPos ); // DEBUG: remove
}

void Enemy::Render(Graphics& gfx, XMMATRIX& mat)
{
	m_sprite->UpdateBuffers(gfx.GetContext());
	m_sprite->Draw(m_transform->GetWorldMatrix(), mat);
}