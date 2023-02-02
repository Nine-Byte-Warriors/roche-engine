#pragma once
#ifndef ENTITYSPAWNER_H
#define ENTITYSPAWNER_H

#include "Entity.h"

struct EntitySpawn
{
	int seed;
	int pos;
};

class EntitySpawner
{
public:
	void AddEntityToSpawn(int seed, int pos);
	void SpawnEntities();
	std::vector<EntityData> GetEntityData();

	void EntitiesAdded();

private:
	std::vector<EntitySpawn> m_entitySpawn;

	std::vector<EntityData> m_vEntityData;
	std::vector<EntityData> m_vEntityDataLive;

	std::string JsonFile = "Entity.json";
};

#endif