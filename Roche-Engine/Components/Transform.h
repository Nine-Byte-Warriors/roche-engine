#pragma once
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Sprite.h"
#include "Vector2f.h"

/// <summary>
/// Container for all game objects.
/// Provides functions to get/set/update position/rotation data.
/// Scaling should be handled on an entity's sprite component.
/// </summary>
class Transform
{
public:
	Transform();
	Transform( const std::shared_ptr<Sprite>& sprite );

	// position
	void SetPositionInit( const Vector2f& pos ) noexcept { m_vPositionInit = m_vPosition = pos; }
	void SetPositionInit( float xPos, float yPos ) noexcept { m_vPositionInit = m_vPosition = { xPos, yPos }; }
	const Vector2f& GetPositionInit() const noexcept { return m_vPositionInit; }

	inline void SetPosition( const Vector2f& pos ) noexcept { m_vPosition = pos; }
	inline void SetPosition( float xPos, float yPos ) noexcept { m_vPosition = { xPos, yPos }; }
	inline const Vector2f& GetPosition() const noexcept { return m_vPosition; }

	void AdjustPosition( const Vector2f& pos ) noexcept { m_vPosition += pos; }
	void AdjustPosition( float xPos, float yPos ) noexcept{ m_vPosition += { xPos, yPos }; }
	void ResetPosition() noexcept { m_vPosition = m_vPositionInit; }

	// rotation
	void SetRotationInit( float rot ) noexcept { m_fRotationInit = m_fRotation = rot; }
	const float& GetRotationInit() const noexcept { return m_fRotationInit; }

	inline void SetRotation( float rot ) noexcept { m_fRotation = rot; }
	inline const float& GetRotation() const noexcept { return m_fRotation; }

	void AdjustRotation( float rot ) noexcept { m_fRotation += rot; }
	void ResetRotation() noexcept { m_fRotation = m_fRotationInit; }

	// scale - only to be used by widgets
	inline void UpdateSprite( const std::shared_ptr<Sprite>& sprite ) noexcept { m_sprite = sprite; }

	// matrix
	inline XMMATRIX GetWorldMatrix() const noexcept { return worldMatrix; }
	inline void SetWorldMatrix( XMMATRIX mat ) noexcept { worldMatrix = mat; }

	void Update();
private:
	std::shared_ptr<Sprite> m_sprite;
	XMMATRIX worldMatrix = XMMatrixIdentity();
	Vector2f m_vPosition, m_vPositionInit;
	float m_fRotation, m_fRotationInit;
	Vector2f m_vScale;
};

#endif