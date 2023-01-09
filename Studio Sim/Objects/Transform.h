#pragma once
#ifndef TRANSFORM_H
#define TRANSFORM_H

/// <summary>
/// Container for all game objects.
/// Provides functions to get/set/update position/rotation/scale data.
/// </summary>
class Transform
{
public:
	const XMVECTOR& GetPositionVector() const noexcept;
	const XMFLOAT3& GetPositionFloat3() const noexcept;
	const XMVECTOR& GetRotationVector() const noexcept;
	const XMFLOAT3& GetRotationFloat3() const noexcept;
	const XMFLOAT3& GetScaleFloat3() const noexcept;

	void SetInitialPosition( const XMFLOAT3& pos ) noexcept;
	void SetInitialPosition( float xPos, float yPos, float zPos ) noexcept;
	const XMFLOAT3& GetInitialPosition() const noexcept { return initialPos; }
	void SetPosition( const XMVECTOR& pos ) noexcept;
	void SetPosition( const XMFLOAT3& pos ) noexcept;
	void SetPosition( float xPos, float yPos, float zPos ) noexcept;
	void AdjustPosition( const XMVECTOR& pos ) noexcept;
	void AdjustPosition( const XMFLOAT3& pos ) noexcept;
	void AdjustPosition( float xPos, float yPos, float zPos ) noexcept;
	void ResetPosition() noexcept;

	void SetInitialRotation( const XMFLOAT3& rot ) noexcept;
	void SetInitialRotation( float xRot, float yRot, float zRot ) noexcept;
	void SetRotation( const XMVECTOR& rot ) noexcept;
	void SetRotation( const XMFLOAT3& rot ) noexcept;
	void SetRotation( float xRot, float yRot, float zRot ) noexcept;
	void AdjustRotation( const XMVECTOR& rot ) noexcept;
	void AdjustRotation( const XMFLOAT3& rot ) noexcept;
	void AdjustRotation( float xRot, float yRot, float zRot ) noexcept;
	void ResetRotation() noexcept;

	void SetInitialScale( const XMFLOAT3& newScale ) noexcept;
	void SetInitialScale( float xScale, float yScale, float zScale = 1.0f ) noexcept;
	void SetScale( const XMFLOAT3& newScale ) noexcept;
	void SetScale( float xScale, float yScale, float zScale = 1.0f ) noexcept;
	void SetScale( float scaleMultiplier ) noexcept;
	void AdjustScale( const XMFLOAT3& newScale ) noexcept;
	void AdjustScale( float xScale, float yScale, float zScale = 1.0f ) noexcept;
	void ResetScale() noexcept;

	inline XMMATRIX GetWorldMatrix() const noexcept { return worldMatrix; }
	inline void SetWorldMatrix( XMMATRIX mat ) noexcept { worldMatrix = mat; }
protected:
	XMMATRIX worldMatrix = XMMatrixIdentity();
	XMFLOAT3 initialPos, initialRot, initialScale;
	XMFLOAT3 position, rotation, scale;
	XMVECTOR posVector, rotVector;
};

#endif