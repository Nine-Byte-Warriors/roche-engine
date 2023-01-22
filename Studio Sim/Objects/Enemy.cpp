#include "stdafx.h"
#include "Enemy.h"
#include "Graphics.h"

#define DEBUG_PATTERN_PATH "Resources\\Patterns\\SlowWiggle.ptn"

Enemy::Enemy()
{
	m_vEnemyPos = new Vector2f();
	m_sprite = std::make_shared<Sprite>();
	m_transform = std::make_shared<Transform>( m_sprite );
	m_physics = std::make_shared<Physics>( m_transform );
	m_agent = std::make_shared<Agent>( m_physics );

	fShootRange = 200.0f;
	m_projectileManager = std::make_shared<ProjectileManager>();
}

void Enemy::Initialize( Graphics& gfx, ConstantBuffer<Matrices>& mat, Sprite::Type type )
{
	m_sprite->Initialize( gfx.GetDevice(), gfx.GetContext(), type, mat );

	float fWidth = m_sprite->GetWidth();
	float fHeight = m_sprite->GetHeight();
	float fRadius = fWidth > fHeight ? fWidth / 2.0f : fHeight / 2.0f;
	m_collider = std::make_shared<CircleCollider>( m_transform, 1, 1, fRadius);

	m_transform->SetPositionInit(gfx.GetWidth() * 0.4f, gfx.GetHeight() / 2);
	m_transform->SetScaleInit(fWidth, fHeight);
	
	m_projectileManager->LoadProjectileData(DEBUG_PATTERN_PATH);
	m_projectileManager->Initialize(gfx, mat);
}

void Enemy::Update( const float dt )
{
	m_sprite->Update( dt );
	m_agent->Update( dt );
	m_transform->Update();
	
	Vector2f vPos = m_transform->GetPosition();
	m_vEnemyPos = &vPos;
	
	Vector2f vTargetPos = m_agent->GetTargetPosition();
	float fDistance = vPos.Distance(vTargetPos);
	if (fShootRange >= fDistance)
		m_projectileManager->SpawnProjectiles(*m_vEnemyPos, vTargetPos);
	
	m_projectileManager->Update(dt);

	EventSystem::Instance()->AddEvent( EVENTID::TargetPosition, m_vEnemyPos ); // DEBUG: remove
}

void Enemy::Render(Graphics& gfx, XMMATRIX& mat)
{
	m_sprite->UpdateBuffers(gfx.GetContext());
	m_sprite->Draw(m_transform->GetWorldMatrix(), mat);

	m_projectileManager->Draw(gfx.GetContext(), mat);
}