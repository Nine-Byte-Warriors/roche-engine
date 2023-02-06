#pragma once
#ifndef ENTITYSPAWNER_H
#define ENTITYSPAWNER_H

#include "Entity.h"

struct EntitySpawn
{
	int seed;
	int tileMapPos;
	Vector2f mapPos;
};

class EntitySpawner
{
public:
	void AddEntityToSpawn(int seed, int tileMapPos, Vector2f mapPos);
	void SpawnEntities();
	std::vector<EntityData> GetEntityData();

	void EntitiesAdded();

	bool IsEntityPosTaken(int pos);
	int GetSpawnEntitiesSize();

	int GetSpawnEntitiesTileMapPos(int num);

private:
	std::vector<EntitySpawn> m_entitySpawn;

	std::vector<EntityData> m_vEntityData;
	std::vector<EntityData> m_vEntityDataLive;

	std::string JsonFile = "Entity.json";
};

#endif