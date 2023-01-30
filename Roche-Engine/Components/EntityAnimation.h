#pragma once
#ifndef ENTITYANIMATION_H
#define ENTITYANIMATION_H

#include "JsonLoading.h"

struct AnimationType
{
	std::string name;
	std::string alias;
	float frameTiming;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(AnimationType, name, alias, frameTiming)

struct AnimationData
{
	std::string name;
	std::vector<int> frameCount;
	std::vector<AnimationType> type;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(AnimationData, name, frameCount, type)


class EntityAnimation
{
public:
	EntityAnimation();
	~EntityAnimation();

	void LoadEntityAnimation(std::string filePath);

	std::string GetAnimationEntityName();
	std::vector<int> GetFrameCount();
	std::string GetAnimationTypeName(int num);
	std::string GetAnimationTypeAlias(int num);
	float GetFrameTiming(int num);
	int GetAnimationTypeSize();

private:
	AnimationData m_animationData;
};

#endif