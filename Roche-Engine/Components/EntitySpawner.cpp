#include "stdafx.h"
#include "EntitySpawner.h"

#define FOLDER_PATH "Resources\\Entity\\"

void EntitySpawner::AddEntityToSpawn(int seed, int tileMapPos, Vector2f mapPos)
{
	EntitySpawn entitySpawn;
	entitySpawn.seed = seed;
	entitySpawn.tileMapPos = tileMapPos;
	entitySpawn.mapPos = mapPos;
	m_entitySpawn.push_back(entitySpawn);

	m_vEntityData.clear();
	JsonLoading::LoadJson(m_vEntityData, FOLDER_PATH + JsonFile);
}

void EntitySpawner::SpawnEntities()
{
	for (int i = 0; i < m_entitySpawn.size(); i++)
	{
		m_vEntityDataLive.push_back(m_vEntityData[m_entitySpawn[i].seed]);
		m_vEntityDataLive[i].position[0] = m_entitySpawn[i].mapPos.x;
		m_vEntityDataLive[i].position[1] = m_entitySpawn[i].mapPos.y;
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

bool EntitySpawner::IsEntityPosTaken(int pos)
{
	for (int i = 0; i < m_entitySpawn.size(); i++)
	{
		if (pos == m_entitySpawn[i].tileMapPos)
		{
			return true;
		}
	}
	return false;
}

int EntitySpawner::GetSpawnEntitiesSize()
{
	return m_entitySpawn.size();
}

int EntitySpawner::GetSpawnEntitiesTileMapPos(int num)
{
	return m_entitySpawn[num].tileMapPos;
}
