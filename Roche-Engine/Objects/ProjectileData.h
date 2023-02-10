#ifndef PROJECTILEDATA_H
#define PROJECTILEDATA_H

#include <string>
#include "JsonLoading.h"

namespace ProjectileData {

	struct ProjectileJSON
	{
		float m_fAmplitude;
		float m_fAngle;
		float m_fDelay;
		float m_fFrequency;
		float m_fLifeTime;
		float m_fSpeed;
		float m_fX;
		float m_fY;
	};
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ProjectileJSON,
		m_fAmplitude, 
		m_fAngle, 
		m_fDelay, 
		m_fFrequency,
		m_fLifeTime, 
		m_fSpeed,
		m_fX, 
		m_fY
	);

	struct ManagerJSON
	{
		bool m_bLoop;
		bool m_bUseTarget;
		bool m_bUseGlobalSpeed;
		float m_fGlobalSpeed;
		float m_fDelay;
		float m_fHeight;
		float m_fWidth;
		int m_iCount;
		std::string m_sID;
		std::string m_sImagePath;
		std::string m_sName;

		std::vector<ProjectileJSON> m_vecProjectiles;
	};
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ManagerJSON, 
		m_bLoop, 
		m_bUseTarget,
		m_bUseGlobalSpeed, 
		m_fGlobalSpeed, 
		m_fDelay, 
		m_fHeight, 
		m_fWidth, 
		m_iCount, 
		m_sID, 
		m_sImagePath, 
		m_sName, 
		m_vecProjectiles
	);
}

#endif