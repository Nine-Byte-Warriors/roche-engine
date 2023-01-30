#include "stdafx.h"
#include "EntityAnimation.h"

EntityAnimation::EntityAnimation()
{
}

EntityAnimation::~EntityAnimation()
{
}

void EntityAnimation::LoadEntityAnimation(std::string filePath)
{
	JsonLoading::LoadJson(m_animationData, filePath);
}

std::string EntityAnimation::GetAnimationEntityName()
{
	return m_animationData.name;
}

std::vector<int> EntityAnimation::GetFrameCount()
{
	return m_animationData.frameCount;
}

std::string EntityAnimation::GetAnimationTypeName(int num)
{
	return m_animationData.type[num].name;
}

std::string EntityAnimation::GetAnimationTypeAlias(int num)
{
	return m_animationData.type[num].alias;
}

float EntityAnimation::GetFrameTiming(int num)
{
	return m_animationData.type[num].frameTiming;
}

int EntityAnimation::GetAnimationTypeSize()
{
	return m_animationData.type.size();
}

