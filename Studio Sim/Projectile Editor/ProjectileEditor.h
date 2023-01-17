#pragma once
#ifndef PROJECTILE_EDITOR_H
#define PROJECTILE_EDITOR_H

#include "EventSystem.h"
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
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ProjectileJSON, m_sID, m_sName, m_sTexture, m_iOrder, m_fSpeed, m_fLifeTime)

struct ProjectileData
{
	std::string m_sName;
	std::string m_sTexture;
	float m_fSpeed;
	float m_fLifeTime;
};

}

class ProjectileEditor : Listener
{
public:
	ProjectileEditor();
	~ProjectileEditor() {}

	static inline std::shared_ptr<ProjectileEditor> CreateEditor() { return std::make_shared<ProjectileEditor>(); }
	
	void SpawnEditorWindow();
	
	void AddToEvent() noexcept;
	void HandleEvent(Event* event) override;
	
private:
	void LoadPattern();
	void SavePattern();

	void ShowPattern();

	bool m_bSaveButton;
	std::string m_sSelectedFile;
	std::string m_sFilePath;
	std::string m_sFileContent;
	
	std::vector< ProjectileData::ProjectileJSON> m_vecProjectiles;
};

#endif // !PROJECTILE_EDITOR_H
