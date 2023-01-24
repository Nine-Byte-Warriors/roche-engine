#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "Transform.h"
#include "EventSystem.h"

/// <summary>
/// Creates a 2D camera which creates an orthographic view on the scene.
/// Used primarily for rendering sprites such as UI components.
/// </summary>
class Camera : public Listener
{
public:
	Camera();
	~Camera();
	void SetProjectionValues( float width, float height, float nearZ, float farZ );
	const XMMATRIX& GetOrthoMatrix() const noexcept;
	const XMMATRIX& GetWorldOrthoMatrix() const noexcept;
	inline std::shared_ptr<Transform> GetTransform() const noexcept { return m_transform; }

	void AddToEvent() noexcept;
	void RemoveFromEvent() noexcept;
	void HandleEvent( Event* event ) override;
private:
	void UpdateMatrix();
	XMMATRIX orthoMatrix;
	XMFLOAT4X4 WorldOrthoMatrix;
	std::shared_ptr<Transform> m_transform;
};

#endif