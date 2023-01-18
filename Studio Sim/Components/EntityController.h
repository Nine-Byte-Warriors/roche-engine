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
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(EntityData, name, texture, type, position, scale)

class EntityController
{
public:
	EntityController();
	~EntityController();

	int GetSize();

	std::string GetTexture(int num);
	std::vector<float> GetPosition(int num);
	std::string GetType(int num);
	std::vector<float> GetScale(int num);

private:
	std::string JsonFile = "Resources\\Entity\\Entity.json";

	std::vector<EntityData> m_entityData;
};

#endif