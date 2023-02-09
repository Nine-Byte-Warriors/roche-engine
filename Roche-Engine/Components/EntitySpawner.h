#pragma once
#ifndef ENTITYSPAWNER_H
#define ENTITYSPAWNER_H

#include "Entity.h"
#include "GameManager.h"

struct EntitySpawn
{
	int seed;
	int tileMapPos;
	Vector2f mapPos;
};

class EntitySpawner : public Listener
{
public:
	EntitySpawner();
	~EntitySpawner();

	void AddEntityToSpawn(int seed, int tileMapPos, Vector2f mapPos);
	void SpawnEntities();
	void SpawnEntity(int num);

	std::vector<EntityData> GetEntityData();

	void EntitiesAdded();

	bool IsEntityPosTaken(int pos);
	int GetSpawnEntitiesSize();

	int GetSpawnEntitiesTileMapPos(int num);

	bool IsPhaseNight();

private:
	void AddToEvent() noexcept;
	void RemoveFromEvent() noexcept;
	void HandleEvent(Event* event) override;

	void RemoveNonEnemiesFromEntityData();

	std::vector<EntitySpawn> m_entitySpawn;

	std::vector<EntityData> m_vEnemyEntityData;
	std::vector<EntityData> m_vEntityDataLive;

	std::string JsonFile = "Entity_Game.json";

	Phase m_currentGamePhase;
};

#endif