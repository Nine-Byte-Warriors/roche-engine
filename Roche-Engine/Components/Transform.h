#pragma once
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Sprite.h"
#include "Vector2f.h"

/// <summary>
/// Container for all game objects.
/// Provides functions to get/set/update position/rotation/scale data.
/// </summary>
class Transform
{
public:
	Transform();
	Transform( const std::shared_ptr<Sprite>& sprite );
	inline std::shared_ptr<Sprite> GetSprite() const noexcept { return m_sprite; }

	// position
	void SetPositionInit( const Vector2f& pos ) noexcept { m_vPositionInit = m_vPosition = pos; }
	void SetPositionInit( float xPos, float yPos ) noexcept { m_vPositionInit = m_vPosition = { xPos, yPos }; }
	const Vector2f& GetPositionInit() const noexcept { return m_vPositionInit; }

	inline void SetPosition( const Vector2f& pos ) noexcept { m_vPosition = pos; }
	inline void SetPosition( float xPos, float yPos ) noexcept { m_vPosition = { xPos, yPos }; }
	inline Vector2f GetPosition() const noexcept { return m_vPosition; }

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

	// scale
	void SetScaleInit( const Vector2f& scale ) noexcept { m_vScaleInit = m_vScale = scale; }
	void SetScaleInit( float xScale, float yScale ) noexcept { m_vScaleInit = m_vScale = { xScale, yScale }; }
	const Vector2f& GetScaleInit() const noexcept { return m_vScaleInit; }

	inline void SetScale( const Vector2f& scale ) noexcept { m_vScale = scale; }
	inline void SetScale( float xScale, float yScale ) noexcept { m_vScale = { xScale, yScale }; }
	inline const Vector2f& GetScale() const noexcept { return m_vScale; }

	void AdjustScale( const Vector2f& scale ) noexcept { m_vScale += scale; }
	void AdjustScale( float xScale, float yScale ) noexcept{ m_vScale += { xScale, yScale }; }
	void ResetScale() noexcept { m_vScale = m_vScaleInit; }

	// matrix
	inline XMMATRIX GetWorldMatrix() const noexcept { return worldMatrix; }
	inline void SetWorldMatrix( XMMATRIX mat ) noexcept { worldMatrix = mat; }

	void Update();
private:
	std::shared_ptr<Sprite> m_sprite;
	XMMATRIX worldMatrix = XMMatrixIdentity();
	Vector2f m_vPosition, m_vPositionInit;
	float m_fRotation, m_fRotationInit;
	Vector2f m_vScale, m_vScaleInit;
};

#endif