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
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(EntityData, name, texture, type, position, scale, identifier, maxFrame)

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

private:
	std::string JsonFile = "Resources\\Entity\\Entity.json";

	std::vector<EntityData> m_entityData;
};

#endif