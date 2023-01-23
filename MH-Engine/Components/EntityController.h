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
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(EntityData, name, texture, type, position, scale, identifier,
	maxFrame, mass, speed, behaviour, colliderShape, colliderRadius, projectilePattern, projectileBullet)

class EntityController
{
public:
	EntityController();
	~EntityController();

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

private:
	std::string JsonFile = "Resources\\Entity\\Entity.json";

	std::vector<EntityData> m_entityData;
};

#endif