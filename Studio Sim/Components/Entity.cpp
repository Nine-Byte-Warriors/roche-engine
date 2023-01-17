#include "stdafx.h"
#include "Entity.h"

Entity::Entity()
{
	JsonLoading::LoadJson(m_entityData, JsonFile);
}

Entity::~Entity()
{
}