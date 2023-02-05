#include "stdafx.h"
#include "EntityController.h"

#define FOLDER_PATH "Resources\\Entity\\"

EntityController::EntityController()
{
	JsonLoading::LoadJson(m_entityData, FOLDER_PATH + JsonFile);
	m_entityDataCopy = m_entityData;
}

void EntityController::SetJsonFile( const std::string& name )
{
	JsonFile = name;
	JsonLoading::LoadJson(m_entityData, FOLDER_PATH + JsonFile);
	m_entityDataCopy = m_entityData;
}

EntityController::~EntityController()
{
}

int EntityController::GetSize()
{
	return m_entityData.size();
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
	m_dead.push_back(num);
}

