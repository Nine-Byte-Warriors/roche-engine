#include "stdafx.h"
#include "ProjectileManager.h"

ProjectileManager::ProjectileManager()
{
	m_fLifeTime = 100.0f; // TODO: should be passed in from Projectile JSON
	m_bRenderProjectiles = false;

	m_vDirection = { 0.0f, 0.0f };
	m_vSpawnPosition = { 0.0f, 0.0f };
	m_vTargetPosition = { 0.0f, 0.0f };

	m_projectile = std::make_shared<Projectile>();
	AddToEvent();
}

void ProjectileManager::Initialize(const Graphics& gfx, ConstantBuffer<Matrices>& mat)
{
	m_projectile->Initialize(gfx, mat, Sprite::Type::Projectile);
	m_projectile->GetTransform()->SetPositionInit( 0.0f, 0.0f );
	m_projectile->GetTransform()->SetScaleInit( m_projectile->GetSprite()->GetWidth(), m_projectile->GetSprite()->GetHeight() );
}

void ProjectileManager::Update( const float dt )
{
	m_bRenderProjectiles = false;
	if ((m_fLifeTime -= dt) >= 0.0f)
	{
		m_bRenderProjectiles = true;
		m_projectile->GetPhysics()->AddForce( m_vDirection );
		m_projectile->Update( dt );
	}
}

void ProjectileManager::Draw( ID3D11DeviceContext* context, XMMATRIX orthoMatrix )
{
	if ( m_bRenderProjectiles )
	{
		m_projectile->GetSprite()->UpdateBuffers( context );
		m_projectile->GetSprite()->Draw( m_projectile->GetTransform()->GetWorldMatrix(), orthoMatrix );
	}
}

void ProjectileManager::SpawnProjectile()
{
	m_vDirection = m_vSpawnPosition.DirectionTo(m_vTargetPosition).Normalised();
	m_projectile->GetTransform()->SetPosition(m_vSpawnPosition);
	m_fLifeTime = m_fLifeTime;
}

void ProjectileManager::AddToEvent() noexcept
{
	EventSystem::Instance()->AddClient(EVENTID::PlayerPosition, this);
	EventSystem::Instance()->AddClient(EVENTID::TargetPosition, this);
	EventSystem::Instance()->AddClient(EVENTID::PlayerFire, this);
}

void ProjectileManager::HandleEvent(Event* event)
{
	switch (event->GetEventID())
	{
	case EVENTID::PlayerPosition: 
		m_vSpawnPosition = *static_cast<Vector2f*>(event->GetData());
		break;
	case EVENTID::TargetPosition: 
		m_vTargetPosition = *static_cast<Vector2f*>(event->GetData());
		break;
	case EVENTID::PlayerFire:
		SpawnProjectile();
		break;
	default:
		break;
	}
}