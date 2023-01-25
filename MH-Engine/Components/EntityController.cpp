#include "stdafx.h"
#include "EntityController.h"

EntityController::EntityController()
{
	JsonLoading::LoadJson(m_entityData, JsonFile);
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

EntityData* EntityController::GetProjectileBullet(int num)
{
	for (int i = 0; i < m_entityData.size(); i++)
	{
		if (m_entityData[i].name == m_entityData[num].projectileBullet)
		{
			return &m_entityData[i];
		}
	}
}

void EntityController::SetEntityData(std::vector<EntityData> entityData)
{
	m_entityData = entityData;
}
