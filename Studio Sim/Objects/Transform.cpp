#include "stdafx.h"
#include "Transform.h"

const XMVECTOR& Transform::GetPositionVector() const noexcept
{
	return posVector;
}

const XMFLOAT3& Transform::GetPositionFloat3() const noexcept
{
	return position;
}

const XMVECTOR& Transform::GetRotationVector() const noexcept
{
	return rotVector;
}

const XMFLOAT3& Transform::GetRotationFloat3() const noexcept
{
	return rotation;
}

const XMFLOAT3& Transform::GetScaleFloat3() const noexcept
{
	return { scale.x, scale.y, scale.z };
}

/// POSITIONS
void Transform::SetInitialPosition( const XMFLOAT3& pos ) noexcept
{
	initialPos = pos;
	SetPosition( pos );
}

void Transform::SetInitialPosition( float xPos, float yPos, float zPos ) noexcept
{
	SetInitialPosition( { xPos, yPos, zPos } );
}

void Transform::SetPosition( const XMVECTOR& pos ) noexcept
{
	XMStoreFloat3( &position, pos );
	posVector = pos;
}

void Transform::SetPosition( const XMFLOAT3& pos ) noexcept
{
	position = pos;
	posVector = XMLoadFloat3( &position );
}

void Transform::SetPosition( float xPos, float yPos, float zPos ) noexcept
{
	SetPosition( XMFLOAT3( xPos, yPos, zPos ) );
}

void Transform::AdjustPosition( const XMVECTOR& pos ) noexcept
{
	posVector += pos;
	XMStoreFloat3( &position, posVector );
}

void Transform::AdjustPosition( const XMFLOAT3& pos ) noexcept
{
	position.x += pos.x;
	position.y += pos.y;
	position.z += pos.z;
	posVector = XMLoadFloat3( &position );
}

void Transform::AdjustPosition( float xPos, float yPos, float zPos ) noexcept
{
	AdjustPosition( XMFLOAT3( xPos, yPos, zPos ) );
}

void Transform::ResetPosition() noexcept
{
	position = initialPos;
}

/// ROTATIONS
void Transform::SetInitialRotation( const XMFLOAT3& rot ) noexcept
{
	initialRot = rot;
	SetRotation( rot );
}

void Transform::SetInitialRotation( float xRot, float yRot, float zRot ) noexcept
{
	SetInitialRotation( { xRot, yRot, zRot } );
}

void Transform::SetRotation( const XMVECTOR& rot ) noexcept
{
	rotVector = rot;
	XMStoreFloat3( &rotation, rot );
}

void Transform::SetRotation( const XMFLOAT3& rot ) noexcept
{
	rotation = rot;
	rotVector = XMLoadFloat3( &rotation );
}

void Transform::SetRotation( float xRot, float yRot, float zRot ) noexcept
{
	SetRotation( XMFLOAT3( xRot, yRot, zRot ) );
}

void Transform::AdjustRotation( const XMVECTOR& rot ) noexcept
{
	rotVector += rot;
	XMStoreFloat3( &rotation, rotVector );
}

void Transform::AdjustRotation( const XMFLOAT3& rot ) noexcept
{
	rotation.x += rot.x;
	rotation.y += rot.y;
	rotation.z += rot.z;

	if ( rotation.x >= XMConvertToRadians( 90.0f ) )
		rotation.x = XMConvertToRadians( 90.0f );

	if ( rotation.x <= XMConvertToRadians( -90.0f ) )
		rotation.x = XMConvertToRadians( -90.0f );

	rotVector = XMLoadFloat3( &rotation );
}

void Transform::AdjustRotation( float xRot, float yRot, float zRot ) noexcept
{
	AdjustRotation( XMFLOAT3( xRot, yRot, zRot ) );
}

void Transform::ResetRotation() noexcept
{
	rotation = initialRot;
}

/// SCALE
void Transform::SetInitialScale( const XMFLOAT3& newScale ) noexcept
{
	SetScale( newScale );
}

void Transform::SetInitialScale( float xScale, float yScale, float zScale ) noexcept
{
	SetInitialScale( XMFLOAT3( xScale, yScale, zScale ) );
}

void Transform::SetScale( const XMFLOAT3& newScale ) noexcept
{
	scale = newScale;
}

void Transform::SetScale( float xScale, float yScale, float zScale ) noexcept
{
	SetScale( XMFLOAT3( xScale, yScale, zScale ) );
}

void Transform::SetScale( float scaleMultiplier ) noexcept
{
	SetScale( scaleMultiplier, scaleMultiplier, scaleMultiplier );
}

void Transform::AdjustScale( const XMFLOAT3& newScale ) noexcept
{
	scale = newScale;
}

void Transform::AdjustScale( float xScale, float yScale, float zScale ) noexcept
{
	scale.x += xScale;
	scale.y += yScale;
	scale.z += zScale;
}

void Transform::ResetScale() noexcept
{
	scale = initialScale;
}