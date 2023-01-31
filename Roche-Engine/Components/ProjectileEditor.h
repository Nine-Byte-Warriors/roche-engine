#pragma once
#ifndef PROJECTILE_EDITOR_H
#define PROJECTILE_EDITOR_H

#include "EventSystem.h"
#include "JsonLoading.h"
#include "ProjectileManager.h"

class ProjectileEditor 
{
public:
	ProjectileEditor();
	~ProjectileEditor() {}

	static inline std::shared_ptr<ProjectileEditor> CreateEditor() { return std::make_shared<ProjectileEditor>(); }
	void Initialise(const Graphics& gfx, ConstantBuffer<Matrices>& mat);
	void Update(const float dt);
	void Draw(ID3D11DeviceContext* context, XMMATRIX orthoMatrix);
	
	void SpawnEditorWindow(const Graphics& gfx, ConstantBuffer<Matrices>& mat);
	
private:
	void LoadPattern();
	void SavePattern();
	void SpawnPosition(Vector2f vWinMax);
	void ShowPattern();
	
	void TestButtons(const Graphics& gfx, ConstantBuffer<Matrices>& mat);
	std::vector<std::shared_ptr<Projectile>> CreateProjectilePool(std::vector<ProjectileData::ProjectileJSON> vecProjectileJsons, float fGlobalSpeed, bool bUseGlobalSpeed);
	ProjectileData::ManagerJSON CreateDefaultManager();
	ProjectileData::ProjectileJSON CreateDefaultProjectile();

	void SaveProjectile();

	void SpawnPattern();

	bool m_bSaveButton;
	bool m_bMidPosSet;
	std::string m_sSelectedFile;
	std::string m_sFilePath;
	std::string m_sFileContent;
	
	std::vector<std::shared_ptr<ProjectileManager>> m_vecProjectileManager;
	std::vector<ProjectileData::ManagerJSON> m_vecManagers;
	std::vector<ProjectileData::ProjectileJSON> m_vecProjectiles;

	int m_iProjectileCount;
	Vector2f m_vSpawnPosition;
};

#endif // !PROJECTILE_EDITOR_H
