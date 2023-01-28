#pragma once
#ifndef ENTITYCONTROLLER_H
#define ENTITYCONTROLLER_H

#include "JsonLoading.h"

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
	bool AI;
	bool projectileSystem;
	bool collider;
	bool bProjectilePattern;
	bool bProjectileBullet;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(EntityData, name, texture, type, position, scale, identifier,
	maxFrame, mass, speed, behaviour, colliderShape, colliderRadius, projectilePattern, projectileBullet,
	AI, projectileSystem, collider, bProjectilePattern, bProjectileBullet)

class EntityController
{
public:
	EntityController();
	~EntityController();
	
	void SetJsonFile( const std::string& name );
	int GetSize();

	std::string GetName(int num);
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

	bool HasAI(int num);
	bool HasProjectileSystem(int num);
	bool HasCollider(int num);

	bool HasProjectileBullet(int num);
	bool HasProjectilePattern(int num);

	bool HasComponentUpdated();

	void UpdateCopy();

private:
	std::string JsonFile = "Entity.json";

	std::vector<EntityData> m_entityData;
	std::vector<EntityData> m_entityDataCopy;

	bool m_bComponentUpdated = false;
};

#endif