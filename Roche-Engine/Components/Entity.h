#pragma once
#ifndef ENTITY_H
#define ENTITY_H

class Graphics;
#include "EntityController.h"
#include "Physics.h"
#include "EventSystem.h"
#include "ProjectileManager.h"
#include "CircleCollider.h"
#include "Agent.h"
#include "EntityAnimation.h"

class Entity
{
public:
	Entity(EntityController& entityController, int EntityNum);
	~Entity();

	void Initialize(const Graphics& gfx, ConstantBuffer<Matrices>& mat);
	void SetProjectileManagerInit(const Graphics& gfx, ConstantBuffer<Matrices>& mat);
	void Update(const float dt);
	void UpdateFromEntityData(const float dt, bool positionLocked);

	inline std::shared_ptr<Agent> GetAI() const noexcept { return m_agent; }
	inline std::shared_ptr<Sprite> GetSprite() const noexcept { return m_sprite; }
	inline std::shared_ptr<Physics> GetPhysics() const noexcept { return m_physics; }
	inline std::shared_ptr<Transform> GetTransform() const noexcept { return m_transform; }
	inline std::shared_ptr<ProjectileManager> GetProjectileManager() const noexcept { return m_projectileManager;	}
	inline std::shared_ptr<Collider> GetCollider() const noexcept {
		if (m_entityController->GetColliderShape(m_iEntityNum) == "Circle")
			return m_colliderCircle;
		else if (m_entityController->GetColliderShape(m_iEntityNum) == "Box")
			return m_colliderBox;		
	};
	inline std::string GetSoundBankName() const noexcept { return m_sSoundBankName; };

	Vector2f GetPos() { return *m_vPosition; }

	std::string GetType();
	void UpdateEntityNum(int num);

private:
	void SetPositionInit();
	void SetScaleInit();
	void SetHealthInit();

	void SetComponents();

	void UpdatePosition();
	void UpdateScale();
	void UpdateAnimation();
	void UpdateRotation();
	void UpdateTexture();
	void SetAnimation();
	void UpdateRowsColumns();

	void UpdateMass();
	void UpdateSpeed();

	void UpdateBehaviour();

	void UpdateProjectilePattern();

	void UpdateColliderRadius();
	void UpdateColliderTrigger();
	void UpdateColliderLayer();
	void UpdateColliderMask();
	void UpdateColliderStatic();
	void UpdateColliderEnabled();

	void UpdateAudio();


	int m_iEntityNum;

	ID3D11Device* m_device;

	Vector2f* m_vPosition;
	float m_fScaleX;
	float m_fScaleY;
	float m_fRotation;
	float m_fHealth;

	int m_iMaxFrameX;
	int m_iMaxFrameY;
	int m_iCurFrameX;
	int m_iCurFrameY;
	std::string m_sTex;
	int m_iRows;
	int m_iColumns;

	std::string m_sBulletTex;
	float m_fBulletScaleX;
	float m_fBulletScaleY;
	int m_iBulletMaxFrameX;
	int m_iBulletMaxFrameY;
	float m_fBulletMass;
	float m_fBulletSpeed;
	std::string m_sBulletPattern;

	float m_fMass;
	float m_fSpeed;

	std::string m_sBehaviour;
	std::string m_sEntityType;

	std::string m_sColliderShape;
	float m_fColliderRadiusX;
	float m_fColliderRadiusY;

	std::shared_ptr<Agent> m_agent;
	std::shared_ptr<Sprite> m_sprite;
	std::shared_ptr<Physics> m_physics;
	std::shared_ptr<Transform> m_transform;
	std::shared_ptr<ProjectileManager> m_projectileManager;
	std::shared_ptr<CircleCollider> m_colliderCircle;
	std::shared_ptr<BoxCollider> m_colliderBox;

	EntityController* m_entityController;

	EntityAnimation m_animation;

	bool m_bAudio;
	std::string m_sSoundBankName;
};

#endif