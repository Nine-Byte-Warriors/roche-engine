#pragma once
#ifndef CAMERA2D_H
#define CAMERA2D_H

#include "Transform.h"

/// <summary>
/// Creates a 2D camera which creates an orthographic view on the scene.
/// Used primarily for rendering sprites such as UI components.
/// </summary>
class Camera2D
{
public:
	Camera2D();
	void SetProjectionValues( float width, float height, float nearZ, float farZ );
	const XMMATRIX& GetOrthoMatrix() const noexcept;
	const XMMATRIX& GetWorldOrthoMatrix() const noexcept;
	void SendWorldOrthoMatrix();
	inline std::shared_ptr<Transform> GetTransform() const noexcept { return m_transform; }
private:
	void UpdateMatrix();
	XMMATRIX orthoMatrix;
	XMFLOAT4X4 WorldOrthoMatrix;
	std::shared_ptr<Transform> m_transform;
};

#endif