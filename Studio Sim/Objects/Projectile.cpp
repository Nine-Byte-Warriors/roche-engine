#include "stdafx.h"
#include "Projectile.h"
#include "Graphics.h"

Projectile::Projectile()
{
	m_sprite = std::make_shared<Sprite>();
	m_transform = std::make_shared<Transform>(m_sprite);
	m_physics = std::make_shared<Physics>(m_transform);

	AddToEvent();
}

void Projectile::Initialize(const Graphics& gfx, ConstantBuffer<Matrices>& mat, Sprite::Type type)
{
	m_sprite->Initialize(gfx.GetDevice(), gfx.GetContext(), type, mat);
}

void Projectile::Update(const float dt)
{
	// TODO: move to ProjectileManager
	if ((m_fLifeTime -= dt) <= 0.0f)
	{
		m_sprite->Update(dt);
		m_physics->Update(dt);
		m_transform->Update();	
	}
}

void Projectile::AddToEvent() noexcept
{
	EventSystem::Instance()->AddClient(EVENTID::LeftMouseClick, this);
	EventSystem::Instance()->AddClient(EVENTID::LeftMouseRelease, this);
}

void Projectile::HandleEvent(Event* event)
{
	if (event->GetEventID() == EVENTID::LeftMouseClick)
		m_vSpawnPosition = *static_cast<Vector2f*>(event->GetData());
	
	if (event->GetEventID() == EVENTID::LeftMouseRelease)
	{
		Vector2f vTargetPosition = *static_cast<Vector2f*>(event->GetData());
		m_vTargetPosition = m_vSpawnPosition.DirectionTo(vTargetPosition);
		
		m_transform->SetPosition(m_vSpawnPosition);
		m_physics->SetVelocity(m_vTargetPosition.Normalised());
		m_fLifeTime = 5.0f;	// TODO: should depend on the projectile/weapon type, allocated by the ProjectileManager
	}
}

void Projectile::SpawnProjectile(Vector2f vSpawnPosition, Vector2f vTargetPosition, float fLifeTime)
{
	Vector2f vDirection = vSpawnPosition
		.DirectionTo(vTargetPosition)
		.Normalised();
	
	m_transform->SetPosition(vSpawnPosition);
	m_physics->SetVelocity(vDirection);
	m_fLifeTime = fLifeTime;
}
