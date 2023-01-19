#include "stdafx.h"
#include "Projectile.h"
#include "Graphics.h"

Projectile::Projectile(float fSpeed, float fLifeTime)
{
	m_vSpawnPosition = Vector2f();
	m_vTargetPosition = Vector2f();
	m_vDirection = Vector2f();
	m_vOffSet = Vector2f();

	m_fSpeed = fSpeed;
	m_fMaxLifeTime = fLifeTime;
	
	m_sprite = std::make_shared<Sprite>();
	m_transform = std::make_shared<Transform>(m_sprite);
	m_physics = std::make_shared<Physics>(m_transform);
}

void Projectile::Initialize(const Graphics& gfx, ConstantBuffer<Matrices>& mat, Sprite::Type type)
{
	m_sprite->Initialize(gfx.GetDevice(), gfx.GetContext(), type, mat);
	m_transform->SetPositionInit(0.0f, 0.0f);
	m_transform->SetScaleInit(m_sprite->GetWidth(), m_sprite->GetHeight());
}

void Projectile::Initialize(const Graphics& gfx, ConstantBuffer<Matrices>& mat, std::string sSpritePath)
{
	m_sprite->Initialize(gfx.GetDevice(), gfx.GetContext(), sSpritePath, mat);
	m_transform->SetPositionInit(0.0f, 0.0f);
	m_transform->SetScaleInit(m_sprite->GetWidth(), m_sprite->GetHeight());
}

void Projectile::Update(const float dt)
{
	if (!IsAlive())
		return;
	
	m_fLifeTime -= dt;

	m_sprite->Update(dt);
	m_physics->AddForce(m_vDirection.Multiply(m_fSpeed));
	m_physics->Update(dt);
	m_transform->Update();
}

void Projectile::Draw(ID3D11DeviceContext* context, XMMATRIX orthoMatrix)
{
	if (!IsAlive())
		return;
	
	m_sprite->UpdateBuffers(context);
	m_sprite->Draw(m_transform->GetWorldMatrix(), orthoMatrix);
}

void Projectile::SpawnProjectile(Vector2f vSpawnPosition, Vector2f vTargetPosition, float fLifeTime)
{
	m_fLifeTime = fLifeTime;
	
	m_vDirection = vSpawnPosition
		.DirectionTo(vTargetPosition)
		.Normalised();
	
	m_transform->SetPosition(vSpawnPosition);

	m_physics->ResetForces();
}

void Projectile::SpawnProjectile(Vector2f vSpawnPosition, float fLifeTime)
{
	m_fLifeTime = fLifeTime <= 0.0f	? m_fMaxLifeTime : fLifeTime;

	m_transform->SetPosition(vSpawnPosition.Add(m_vOffSet));

	m_physics->ResetForces();
}