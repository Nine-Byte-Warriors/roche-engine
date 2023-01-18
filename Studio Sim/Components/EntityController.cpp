#include "stdafx.h"
#include "EntityController.h"

EntityController::EntityController()
{
	//EntityData temp;
	//temp.name = "Player";
	//temp.texture = "Resources\\Textures\\carrot_ss.png";
	//temp.type = "Player";
	//temp.position.push_back(0.0f);
	//temp.position.push_back(0.1f);

	//m_entityData.push_back(temp);

	//JsonLoading::SaveJson(m_entityData, JsonFile);

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

