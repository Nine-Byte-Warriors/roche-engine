#include "stdafx.h"
#include "EntityController.h"

#define FOLDER_PATH "Resources\\Entity\\"

EntityController::EntityController()
{
	//EntityData temp;
	//temp.name = "Player";
	//temp.texture = "Resources\\Textures\\carrot_ss.png";
	//temp.type = "Player";
	//temp.position.push_back(0.0f);
	//temp.position.push_back(0.1f);
	//temp.scale.push_back(0.0f);
	//temp.scale.push_back(0.1f);
	//temp.frame.push_back(1);
	//temp.frame.push_back(1);
	//temp.identifier = 0;

	//m_entityData.push_back(temp);

	//JsonLoading::SaveJson(m_entityData, JsonFile);
	JsonLoading::LoadJson(m_entityData, FOLDER_PATH + JsonFile);
	AddToEvent();
}

void EntityController::SetJsonFile( const std::string& name )
{
	JsonFile = name;
	JsonLoading::LoadJson( m_entityData, FOLDER_PATH + JsonFile );
}

EntityController::~EntityController()
{
	RemoveFromEvent();
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

std::string EntityController::GetBehaviour(int num)
{
	return m_entityData[num].behaviour;
}

void EntityController::SetEntityData(std::vector<EntityData> entityData)
{
	m_entityData = entityData;
}

void EntityController::AddToEvent() noexcept
{
	EventSystem::Instance()->AddClient(EVENTID::PlayerUp, this);
	EventSystem::Instance()->AddClient(EVENTID::PlayerLeft, this);
	EventSystem::Instance()->AddClient(EVENTID::PlayerDown, this);
	EventSystem::Instance()->AddClient(EVENTID::PlayerRight, this);
}

void EntityController::RemoveFromEvent() noexcept
{
	EventSystem::Instance()->RemoveClient(EVENTID::PlayerUp, this);
	EventSystem::Instance()->RemoveClient(EVENTID::PlayerLeft, this);
	EventSystem::Instance()->RemoveClient(EVENTID::PlayerDown, this);
	EventSystem::Instance()->RemoveClient(EVENTID::PlayerRight, this);
}

void EntityController::HandleEvent(Event* event)
{
	m_eventId = event->GetEventID();
}

EVENTID EntityController::GetEventId()
{
	return m_eventId;
}
