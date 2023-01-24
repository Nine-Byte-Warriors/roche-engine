#pragma once
#ifndef ENTITYCONTROLLER_H
#define ENTITYCONTROLLER_H

#include "JsonLoading.h"
#include "EventSystem.h"


struct EntityData
{
	std::string name;
	std::string texture;
	std::string type;
	std::vector<float> position;
	std::vector<float> scale;
	int identifier;
	std::vector<int> maxFrame;
	float mass;
	std::string behaviour;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(EntityData, name, texture, type, position, scale, identifier, maxFrame, mass, behaviour)

class EntityController : public Listener
{
public:
	EntityController();
	~EntityController();

	int GetSize();

	std::string GetType(int num);
	std::string GetTexture(int num);
	std::vector<float> GetPosition(int num);
	std::vector<float> GetScale(int num);
	std::vector<int> GetMaxFrame(int num);
	float GetMass(int num);
	std::string GetBehaviour(int num);

	void SetEntityData(std::vector<EntityData> entityData);

	void AddToEvent() noexcept;
	void RemoveFromEvent() noexcept;
	void HandleEvent(Event* event) override;

	EVENTID GetEventId();

private:
	std::string JsonFile = "Resources\\Entity\\Entity.json";

	std::vector<EntityData> m_entityData;

	EVENTID m_eventId;
};

#endif