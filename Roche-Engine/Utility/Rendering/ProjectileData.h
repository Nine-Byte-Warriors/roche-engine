#ifndef PROJECTILEDATA_H
#define PROJECTILEDATA_H

#include <string>
#include "JsonLoading.h"

namespace ProjectileData {

	struct ProjectileJSON
	{
		float m_fSpeed;
		float m_fLifeTime;
		float m_fX;
		float m_fY;
		float m_fDelay;
		float m_fAngle;
		float m_fAmplitude;
		float m_fFrequency;
		//onDeathEvent or endOfLifeTimeEvent
	};
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ProjectileJSON, m_fSpeed, m_fLifeTime, m_fDelay, m_fX, m_fY, m_fAngle, m_fAmplitude, m_fFrequency);

	struct ManagerJSON
	{
		std::string m_sID;
		std::string m_sName;
		std::string m_sImagePath;
		float m_fWidth;
		float m_fHeight;
		float m_fDelay;
		int m_iCount;
		bool m_bUseGlobalSpeed;
		float m_fGlobalSpeed;
		bool m_bLoop;
		//shapeIdentifier/formation/pattern

		std::vector<ProjectileJSON> m_vecProjectiles;
	};
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ManagerJSON, m_sID, m_sName, m_sImagePath, m_fWidth, m_fHeight, m_fDelay, m_iCount, m_bLoop, m_vecProjectiles);
}

#endif