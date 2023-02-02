#include "stdafx.h"
#include "EntitySpawner.h"

#define FOLDER_PATH "Resources\\Entity\\"

void EntitySpawner::AddEntityToSpawn(int seed, int pos)
{
	EntitySpawn entitySpawn;
	entitySpawn.seed = seed;
	entitySpawn.pos = pos;
	m_entitySpawn.push_back(entitySpawn);

	m_vEntityData.clear();
	JsonLoading::LoadJson(m_vEntityData, FOLDER_PATH + JsonFile);
}

void EntitySpawner::SpawnEntities()
{
	for (int i = 0; i < m_entitySpawn.size(); i++)
	{
		m_vEntityDataLive.push_back(m_vEntityData[m_entitySpawn[i].seed]);
	}

	m_entitySpawn.clear();
}

std::vector<EntityData> EntitySpawner::GetEntityData()
{
	return m_vEntityDataLive;
}

void EntitySpawner::EntitiesAdded()
{
	m_vEntityDataLive.clear();
}
