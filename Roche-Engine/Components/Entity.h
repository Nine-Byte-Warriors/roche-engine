#pragma once
#ifndef ENTITY_H
#define ENTITY_H

class Graphics;
#include "Agent.h"
#include "Physics.h"
#include "EntityController.h"
#include "ProjectileManager.h"
#include "PlayerController.h"
#include "EntityAnimation.h"
#include "CircleCollider.h"
#include "Inventory.h"
#include "Health.h"
#include "Emitter.h"
#include "ShopItem.h"
#include "LevelTrigger.h"
#include "CollisionHandler.h"
#include "CarrotEnemy.h"

class Entity
{
public:
	Entity(EntityController& entityController, int EntityNum);
	~Entity();

	void Initialize(const Graphics& gfx, ConstantBuffer<Matrices>& mat);
	void SetProjectileManagerInit(const Graphics& gfx, ConstantBuffer<Matrices>& mat);
	void Update(const float dt);
	void UpdateFromEntityData(const float dt, bool positionLocked);

	inline void SetCollisionHandler(CollisionHandler* handler) { 
		m_collisionHandler = handler; }

	inline std::shared_ptr<Inventory> GetInventory() const noexcept{ return m_inventory; }
	inline std::shared_ptr<Agent> GetAI() const noexcept { return m_agent; }
	inline std::shared_ptr<Health> GetHealth() const noexcept { return m_health; }
	inline std::shared_ptr<Sprite> GetSprite() const noexcept { return m_sprite; }
	inline std::shared_ptr<Physics> GetPhysics() const noexcept { return m_physics; }
	inline std::shared_ptr<Transform> GetTransform() const noexcept { return m_transform; }
	inline std::vector<std::shared_ptr<ProjectileManager>> GetProjectileManagers() const noexcept { return m_vecProjectileManagers; }
	inline std::shared_ptr<Collider> GetCollider() const noexcept {
		if (m_entityController->GetColliderShape(m_iEntityNum) == "Circle")
			return m_colliderCircle;
		else if (m_entityController->GetColliderShape(m_iEntityNum) == "Box")
			return m_colliderBox;
	};
	inline std::string GetSoundBankName() const noexcept { return m_sSoundBankName; };
	inline std::shared_ptr<Emitter> GetEmitter() const noexcept { return m_emitter; }

	Vector2f GetPos() { return *m_vPosition; }

	std::string GetType();
	void UpdateEntityNum(int num);

	void CheckAliveStatus();
	void TomatoKamikaze();

private:
	void SetPositionInit();
	void SetScaleInit();

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

	void UpdateAudio();


	int m_iEntityNum;

	float m_fEntityHealth = 100.0;

	ID3D11Device* m_device;
	ID3D11DeviceContext* m_context;
	ConstantBuffer<Matrices>* m_mat;

	Vector2f* m_vPosition;
	float m_fScaleX;
	float m_fScaleY;
	float m_fRotation;

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
	std::shared_ptr<Health> m_health;
	std::shared_ptr<Sprite> m_sprite;
	std::shared_ptr<Physics> m_physics;
	std::shared_ptr<Transform> m_transform;
	std::shared_ptr<BoxCollider> m_colliderBox;
	std::shared_ptr<CircleCollider> m_colliderCircle;
	std::vector<std::shared_ptr<ProjectileManager>> m_vecProjectileManagers;
	std::shared_ptr<BaseController> m_pController;
	std::shared_ptr<ShopItem> m_shopItem;
	std::shared_ptr<LevelTrigger> m_levelTrigger;
	std::shared_ptr<Inventory>m_inventory;
	std::shared_ptr<Emitter> m_emitter;
	std::shared_ptr<CarrotEnemy> m_carrotEnemy;

	EntityController* m_entityController;

	CollisionHandler* m_collisionHandler;

	EntityAnimation m_animation;

	bool m_bAudio;
	std::string m_sSoundBankName;
};

#endif