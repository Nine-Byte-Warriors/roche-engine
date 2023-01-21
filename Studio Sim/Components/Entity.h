#pragma once
#ifndef ENTITY_H
#define ENTITY_H

class Graphics;
#include "EntityController.h"
#include "Physics.h"
#include "EventSystem.h"
#include "ProjectileManager.h"
#include "Collider.h"

class Entity : public Listener
{
public:
	Entity(EntityController& entityController, int EntityNum);

	void Initialize(const Graphics& gfx, ConstantBuffer<Matrices>& mat);
	void Update(const float dt);
	void UpdateFromEntityData(const float dt);

	void AddToEvent() noexcept;
	void HandleEvent(Event* event) override;

	inline std::shared_ptr<Sprite> GetSprite() const noexcept { return m_sprite; }
	inline std::shared_ptr<Physics> GetPhysics() const noexcept { return m_physics; }
	inline std::shared_ptr<Transform> GetTransform() const noexcept { return m_transform; }
	inline std::shared_ptr<ProjectileManager> GetProjectileManager() const noexcept { return m_projectileManager; }
	inline std::shared_ptr<BoxCollider> GetCollider() const noexcept { return m_collider; };

private:
	void SetPositionInit();
	void SetScaleInit();
	void SetFrameInit();

	void UpdatePosition();
	void UpdateScale();
	void UpdateFrame();
	void UpdateTexture();
	void UpdateMass();

	int m_iEntityNum;

	ID3D11Device* m_device;

	Vector2f* m_vPosition;
	float m_fScaleX;
	float m_fScaleY;
	int m_iMaxFrameX;
	int m_iMaxFrameY;
	std::string m_sTex;
	float m_fMass;

	std::shared_ptr<Sprite> m_sprite;
	std::shared_ptr<Physics> m_physics;
	std::shared_ptr<Transform> m_transform;
	std::shared_ptr<ProjectileManager> m_projectileManager;
	std::shared_ptr<BoxCollider> m_collider;

	EntityController* m_entityController;
};

#endif