#include "stdafx.h"
#include "EntityController.h"

#define FOLDER_PATH "Resources\\Entity\\"

EntityController::EntityController()
{
}

void EntityController::SetJsonFile( const std::string& name )
{
	AddToEvent();

	JsonLoading::LoadJson(m_entityData, FOLDER_PATH + name);
	JsonLoading::LoadJson(m_entityEnemyData, FOLDER_PATH + name);
	RemoveNonEnemiesFromEntityEnemyData();

#ifdef _DEBUG
#else
	RemoveEnemiesFromEntityData();
#endif
	m_entityDataCopy = m_entityData;
}

EntityController::~EntityController()
{
	EventSystem::Instance()->RemoveClient(EVENTID::EnemyDeath, this);
	EventSystem::Instance()->RemoveClient(EVENTID::GameLevelChangeEvent, this);
}

int EntityController::GetSize()
{
	return m_entityData.size();
}

int EntityController::GetEntityNumFromName(std::string name)
{
	for (int i = 0; i < m_entityData.size(); i++)
	{
		if (GetName(i).contains(name) && !GetName(i).contains("Item"))
		{
			return i;
		}
	}
	return 0;
}

int EntityController::GetEntityEnemyNumFromName(std::string name)
{
	for (int i = 0; i < m_entityEnemyData.size(); i++)
	{
		if (m_entityEnemyData[i].name.contains(name) && !m_entityEnemyData[i].name.contains("Item"))
		{
			return i;
		}
	}
	return 0;
}

std::string EntityController::GetTexture(int num)
{
	return m_entityData[num].texture;
}

std::vector<float> EntityController::GetPosition(int num)
{
	return m_entityData[num].position;
}

float EntityController::GetRotation(int num)
{
	return m_entityData[num].rotation;
}

float EntityController::GetHealth(int num)
{
	return m_entityData[num].health;
}

std::string EntityController::GetName(int num)
{
	return m_entityData[num].name;
}

std::string EntityController::GetType(int num)
{
	return m_entityData[num].type;
}

std::vector<float> EntityController::GetScale(int num)
{
	return m_entityData[num].scale;
}

std::vector<int> EntityController::GetMaxFrame(int num)
{
	return m_entityData[num].maxFrame;
}

std::string EntityController::GetAnimationFile(int num)
{
	return m_entityData[num].animationPath;
}

std::string EntityController::GetAnimationType(int num)
{
	return m_entityData[num].animationType;
}

Vector2f EntityController::GetEnemyWidthHeight(int num)
{
	if (m_entityEnemyData.size() != 0)
	{
		Vector2f widthHeight = Vector2f(m_entityEnemyData[num].scale[0], m_entityEnemyData[num].scale[1]);
		return widthHeight;
	}
	return Vector2f(0, 0);
}

int EntityController::GetRows(int num)
{
	return m_entityData[num].rows;
}

int EntityController::GetColumns(int num)
{
	return m_entityData[num].columns;
}

float EntityController::GetMass(int num)
{
	return m_entityData[num].mass;
}

float EntityController::GetSpeed(int num)
{
	return m_entityData[num].speed;
}

std::string EntityController::GetBehaviour(int num)
{
	return m_entityData[num].behaviour;
}
//Collider Data
std::string EntityController::GetColliderShape(int num)
{
	return m_entityData[num].colliderShape;
}

std::vector<float> EntityController::GetColliderRadius(int num)
{
	return m_entityData[num].colliderRadius;
}

bool EntityController::GetColliderTrigger(int num)
{
	return m_entityData[num].bColliderTrigger;
}

bool EntityController::GetColliderEnabled(int num)
{
	return m_entityData[num].bColliderEnabled;
}

std::string EntityController::GetColliderLayer(int num)
{
	return m_entityData[num].sColliderLayer;
}

bool EntityController::GetColliderStatic(int num)
{
	return m_entityData[num].bColliderStatic;
}

std::vector<bool> EntityController::GetColliderMask(int num)
{
	bool decoration = m_entityData[num].bColliderInteractDecoration;
	bool player = m_entityData[num].bColliderInteractPlayer;
	bool enemy = m_entityData[num].bColliderInteractEnemy;
	bool projectile = m_entityData[num].bColliderInteractProjectile;

	//dec , player, enemy, proj
	std::vector<bool> colliderMask = {decoration, player, enemy, projectile};
	return colliderMask;
}

EntityData* EntityController::GetProjectileBullet(int num)
{
	for (int i = 0; i < m_entityData.size(); i++)
	{
		if (m_entityData[i].name == m_entityData[num].projectileBullet)
		{
			return &m_entityData[i];
		}
	}

	return &m_entityData[num];
}

std::string EntityController::GetSoundBankName(int num)
{
	return m_entityData[num].soundBankName;
}



void EntityController::SetEntityData(std::vector<EntityData> entityData)
{
	m_entityData = entityData;
}

void EntityController::AddEntityData(std::vector<EntityData> entityData)
{
	for (int i = 0; i < entityData.size(); i++)
	{
		m_entityData.push_back(entityData[i]);
	}
}

void EntityController::AddEntityData(EntityData entityData)
{
	m_entityData.push_back(entityData);
}

bool EntityController::HasAI(int num)
{
	return m_entityData[num].AI;
}

bool EntityController::HasProjectileSystem(int num)
{
	return m_entityData[num].projectileSystem;
}

bool EntityController::HasCollider(int num)
{
	return m_entityData[num].collider;
}

bool EntityController::HasAnimation(int num)
{
	return m_entityData[num].animation;
}

bool EntityController::HasProjectileBullet(int num)
{
	return m_entityData[num].bProjectileBullet;
}

bool EntityController::HasProjectilePattern(int num)
{
	return m_entityData[num].bProjectilePattern;
}

bool EntityController::HasAudio(int num)
{
	return m_entityData[num].audio;
}

bool EntityController::HasComponentUpdated()
{
	for (int i = 0; i < m_entityData.size(); i++)
	{
		bool componentUpdated =
			m_entityDataCopy[i].collider != m_entityData[i].collider ||
			m_entityDataCopy[i].AI != m_entityData[i].AI ||
			m_entityDataCopy[i].projectileSystem != m_entityData[i].projectileSystem ||
			m_entityDataCopy[i].bProjectilePattern != m_entityData[i].bProjectilePattern ||
			m_entityDataCopy[i].animation != m_entityData[i].animation ||
			m_entityDataCopy[i].bProjectileBullet != m_entityData[i].bProjectileBullet;
			m_entityDataCopy[i].audio != m_entityData[i].audio;

		if (componentUpdated)
		{
			m_entityDataCopy = m_entityData;
			return true;
		}
	}
	return false;
}

void EntityController::UpdateCopy()
{
	m_entityDataCopy = m_entityData;
}

void EntityController::SetDead(int num)
{
	if (m_entityData.size() > num && m_entityData[num].type == "Enemy")
	{
		m_dead.push_back(num);
		m_entityData.erase(m_entityData.begin() + num);
	}
}

void EntityController::RemoveEnemiesFromEntityData()
{
	int num = 0;
	int entityDataSize = m_entityData.size();
	for (int i = 0; i < entityDataSize; i++)
	{
		if (GetType(num) == "Enemy")
		{
			m_entityData.erase(m_entityData.begin() + num);
			num--;
		}
		num++;
	}
}
void EntityController::ClearDead()
{
	m_dead.clear();
}

void EntityController::RemoveNonEnemiesFromEntityEnemyData()
{
	int num = 0;
	int entityDataSize = m_entityEnemyData.size();
	for (int i = 0; i < entityDataSize; i++)
	{
		if (m_entityEnemyData[num].type != "Enemy")
		{
			m_entityEnemyData.erase(m_entityEnemyData.begin() + num);
			num--;
		}
		num++;
	}
}


std::vector<int> EntityController::GetDead()
{
	return m_dead;
}

void EntityController::HandleEvent(Event* event)
{
	switch (event->GetEventID())
	{
	case EVENTID::EnemyDeath:
	{
		int* entityNum = static_cast<int*>(event->GetData());
		SetDead(*entityNum);
		break;
	}
	default:
		break;
	}
}

void EntityController::AddToEvent() noexcept
{
	EventSystem::Instance()->AddClient(EVENTID::EnemyDeath, this);
}