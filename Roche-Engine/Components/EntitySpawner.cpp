#include "stdafx.h"
#include "EntitySpawner.h"

#define FOLDER_PATH "Resources\\Entity\\"

EntitySpawner::EntitySpawner()
{
	JsonLoading::LoadJson(m_vEnemyEntityData, FOLDER_PATH + JsonFile);
	RemoveNonEnemiesFromEntityData();
	AddToEvent();
}

EntitySpawner::~EntitySpawner()
{
	RemoveFromEvent();
}

void EntitySpawner::AddEntityToSpawn(int seed, int tileMapPos, Vector2f mapPos)
{
	EntitySpawn entitySpawn;
	entitySpawn.seed = seed;
	entitySpawn.tileMapPos = tileMapPos;
	entitySpawn.mapPos = mapPos;
	m_entitySpawn.push_back(entitySpawn);
}

void EntitySpawner::SpawnEntities()
{
	for (int i = 0; i < m_entitySpawn.size(); i++)
	{
		m_vEntityDataLive.push_back(m_vEnemyEntityData[m_entitySpawn[i].seed]);
		m_vEntityDataLive[i].position[0] = m_entitySpawn[i].mapPos.x;
		m_vEntityDataLive[i].position[1] = m_entitySpawn[i].mapPos.y;
	}

	m_entitySpawn.clear();
}

void EntitySpawner::SpawnEntity(int num)
{
	m_vEntityDataLive.push_back(m_vEnemyEntityData[m_entitySpawn[num].seed]);
	m_vEntityDataLive[num].position[0] = m_entitySpawn[num].mapPos.x;
	m_vEntityDataLive[num].position[1] = m_entitySpawn[num].mapPos.y;

	m_entitySpawn.erase(m_entitySpawn.begin() + num);
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

bool EntitySpawner::IsPhaseNight()
{
	if (m_currentGamePhase == Phase::NightPhase)
	{		
		return true;
	}
	if (m_currentGamePhase == Phase::DayPhase)
	{
		return false;
	}
	return false;
}

void EntitySpawner::AddToEvent() noexcept
{
	EventSystem::Instance()->AddClient(EVENTID::CurrentPhase, this);
}

void EntitySpawner::RemoveFromEvent() noexcept
{
	EventSystem::Instance()->RemoveClient(EVENTID::CurrentPhase, this);
}

void EntitySpawner::HandleEvent(Event* event)
{
	switch (event->GetEventID())
	{
	case EVENTID::CurrentPhase:
	{
		m_currentGamePhase = *static_cast<Phase*>(event->GetData());
	}
	break;
	}
}

void EntitySpawner::RemoveNonEnemiesFromEntityData()
{
	int num = 0;
	int entityDataSize = m_vEnemyEntityData.size();
	for (int i = 0; i < entityDataSize; i++)
	{
		if (m_vEnemyEntityData[num].type != "Enemy")
		{
			m_vEnemyEntityData.erase(m_vEnemyEntityData.begin() + num);
			num--;
		}
		num++;
	}
}
