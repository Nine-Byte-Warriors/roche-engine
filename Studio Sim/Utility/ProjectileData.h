#include <string>
#include "JsonLoading.h"

namespace ProjectileData {

	struct ProjectileJSON
	{
		std::string m_sID;
		std::string m_sName;
		std::string m_sTexture;
		int m_iOrder;
		float m_fSpeed;
		float m_fLifeTime;
		float m_fAngle;
		float m_fX;
		float m_fY;
		int m_iCount;
	};
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ProjectileJSON,
		m_sID, m_sName, m_sTexture, 
		m_iOrder, m_fSpeed, m_fLifeTime, m_fAngle, m_fX, m_fY, m_iCount)
}