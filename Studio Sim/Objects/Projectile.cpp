#include "stdafx.h"
#include "Projectile.h"
#include "Graphics.h"

Projectile::Projectile()
{
	m_sprite = std::make_shared<Sprite>();
	m_transform = std::make_shared<Transform>(m_sprite);
	m_physics = std::make_shared<Physics>(m_transform);
}

void Projectile::Initialize(const Graphics& gfx, ConstantBuffer<Matrices>& mat, Sprite::Type type)
{
	m_sprite->Initialize(gfx.GetDevice(), gfx.GetContext(), type, mat);
}

void Projectile::Update(const float dt)
{
	m_sprite->Update(dt);
	m_physics->Update(dt);
	m_transform->Update();
}