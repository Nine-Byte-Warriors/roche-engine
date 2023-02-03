#include "stdafx.h"
#include "Transform.h"

Transform::Transform()
{
	m_vPosition = { 0.0f, 0.0f };
	m_fRotation = 0.0f;
	m_vScale = { 0.0f, 0.0f };
}

Transform::Transform( const std::shared_ptr<Sprite>& sprite ) : m_sprite( sprite )
{
	m_vPosition = { 0.0f, 0.0f };
	m_fRotation = 0.0f;
	m_vScale = { m_sprite->GetWidth(), m_sprite->GetHeight() };
}

void Transform::Update()
{
	if ( m_sprite != nullptr )
		m_vScale = { m_sprite->GetWidth(), m_sprite->GetHeight() };

	worldMatrix =
		XMMatrixScaling( m_vScale.x, m_vScale.y, 1.0f ) *
		XMMatrixRotationRollPitchYaw( 0.0f, 0.0f, m_fRotation ) *
		XMMatrixTranslation(
			m_vPosition.x + m_vScale.x / 2.0f,
			m_vPosition.y + m_vScale.y / 2.0f,
			0.0f
		);
}