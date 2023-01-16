#pragma once
#ifndef PROJECTILE_EDITOR_H
#define PROJECTILE_EDITOR_H

#include "EventSystem.h"
#include "JsonLoading.h"

struct ProjectileJSON
{
	std::string m_sID;
	std::string m_sName;
	std::string m_sTexture;
	int order;
	float m_fSpeed;
	float m_fLifeTime;
};

struct ProjectileData
{
	std::string m_sName;
	std::string m_sTexture;
	float m_fSpeed;
	float m_fLifeTime;
};

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

	bool m_bLoadButton;
	std::string m_sSelectedFile;
	std::string m_sFilePath;
	std::string m_sFileContent;

	bool m_bSaveButton;
};

#endif // !PROJECTILE_EDITOR_H
