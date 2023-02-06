#include "stdafx.h"
#include "Transform.h"

Transform::Transform( const std::shared_ptr<Sprite>& sprite ) : m_sprite( sprite )
{
	m_vPosition = { 0.0f, 0.0f };
	m_fRotation = 0.0f;
}

void Transform::Update()
{
	worldMatrix =
		XMMatrixScaling( m_sprite->GetWidth(), m_sprite->GetHeight(), 1.0f ) *
		XMMatrixRotationRollPitchYaw( 0.0f, 0.0f, m_fRotation ) *
		XMMatrixTranslation(
			m_vPosition.x + m_sprite->GetWidth() / 2.0f,
			m_vPosition.y + m_sprite->GetHeight() / 2.0f,
			0.0f
		);
}