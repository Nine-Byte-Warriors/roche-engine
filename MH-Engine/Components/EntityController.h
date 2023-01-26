#pragma once
#ifndef ENTITYCONTROLLER_H
#define ENTITYCONTROLLER_H

#include "JsonLoading.h"
#include "EventSystem.h"

struct EntityData
{
	std::string name;
	std::string texture;
	std::string type;
	std::vector<float> position;
	std::vector<float> scale;
	int identifier;
	std::vector<int> maxFrame;
	float mass;
	float speed;
	std::string behaviour;
	std::string colliderShape;
	std::vector<float> colliderRadius;
	std::string projectilePattern;
	std::string projectileBullet;
	bool sprite;
	bool physics;
	bool AI;
	bool projectileSystem;
	bool collider;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(EntityData, name, texture, type, position, scale, identifier,
	maxFrame, mass, speed, behaviour, colliderShape, colliderRadius, projectilePattern, projectileBullet,
	sprite, physics, AI, projectileSystem, collider)

class EntityController
{
public:
	EntityController();
	~EntityController();
	
	void SetJsonFile( const std::string& name );
	int GetSize();

	std::string GetType(int num);
	std::string GetTexture(int num);
	std::vector<float> GetPosition(int num);
	std::vector<float> GetScale(int num);
	std::vector<int> GetMaxFrame(int num);

	float GetMass(int num);
	float GetSpeed(int num);

	std::string GetBehaviour(int num);

	std::string GetColliderShape(int num);
	std::vector<float> GetColliderRadius(int num);

	EntityData* GetProjectileBullet(int num);

	void SetEntityData(std::vector<EntityData> entityData);

	bool HasSprite(int num);
	bool HasPhysics(int num);
	bool HasAI(int num);
	bool HasProjectileSystem(int num);
	bool HasCollider(int num);

	bool HasComponentUpdated();

private:
	std::string JsonFile = "Entity.json";

	std::vector<EntityData> m_entityData;
	std::vector<EntityData> m_entityDataCopy;

	bool m_bComponentUpdated = false;
};

#endif