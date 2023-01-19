#include <string>
#include "JsonLoading.h"

namespace ProjectileData {

	struct ProjectileJSON
	{
		float m_fSpeed;
		float m_fLifeTime;
		float m_fAngle;
		float m_fX;
		float m_fY;
		float m_fWaveAngle;
		//onDeathEvent or endOfLifeTimeEvent
	};
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ProjectileJSON, m_fSpeed, m_fLifeTime, m_fAngle, m_fX, m_fY, m_fWaveAngle);
	
	struct ManagerJSON
	{
		std::string m_sID;
		std::string m_sName;
		std::string m_sImagePath;
		float m_fDelay;
		int m_iCount;
		//shapeIdentifier/formation/pattern

		std::vector<ProjectileJSON> m_vecProjectiles;
	};
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ManagerJSON, m_sID, m_sName, m_sImagePath, m_fDelay, m_iCount, m_vecProjectiles);
}