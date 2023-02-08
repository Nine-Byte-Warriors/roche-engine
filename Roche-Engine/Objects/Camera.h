#pragma once
#ifndef CAMERA_H
#define CAMERA_H

/// <summary>
/// Creates a 2D camera which creates an orthographic view on the scene.
/// Used primarily for rendering sprites such as UI components.
/// </summary>
class Camera : public Listener
{
public:
	Camera() { AddToEvent(); }
	~Camera() { RemoveFromEvent(); }

	void SetProjectionValues(float width, float height, float nearZ, float farZ);
	inline const XMMATRIX& GetWorldMatrix() const noexcept { return m_mWorldMatrix; }
	inline const XMMATRIX& GetOrthoMatrix() const noexcept { return m_mOrthoMatrix; }
	inline const XMMATRIX& GetWorldOrthoMatrix() const noexcept { return m_mWorldMatrix * m_mOrthoMatrix; }

	inline bool GetIsLockedToPlayer() const noexcept { return m_bLockedToPlayer; }
	inline bool SetIsLockedToPlayer(bool locked) noexcept { m_bLockedToPlayer = locked; }

	inline XMFLOAT2 GetPosition() noexcept { return m_vPosition; }
	inline XMFLOAT2 GetInitPosition() noexcept { return m_vInitPosition; }

	void Update(const float dt);
	void SpawnControlWindow();
private:
	void AddToEvent() noexcept;
	void RemoveFromEvent() noexcept;
	void HandleEvent(Event* event) override;

	float m_fSpeed = 5.0f;
	bool m_bLockedToPlayer = false;
	XMFLOAT2 m_vSizeOfScreen, m_vPosition, m_vInitPosition;
	XMMATRIX m_mOrthoMatrix, m_mWorldMatrix;

	XMFLOAT2 m_fMousePos;
};

#endif