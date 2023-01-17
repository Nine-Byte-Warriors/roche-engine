#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include "JsonLoading.h"

struct EntityData
{
	std::string name;
	std::string texture;
	int type;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(EntityData, name, texture, type)

class Entity
{
public:
	Entity();
	~Entity();

private:
	std::string JsonFile = "Resources\\Entity\\Entity.json";

	std::vector<EntityData> m_entityData;
};

#endif