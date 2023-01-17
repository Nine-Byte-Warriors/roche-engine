#pragma once
#ifndef PROJECTILE_EDITOR_H
#define PROJECTILE_EDITOR_H

#include "EventSystem.h"
#include "JsonLoading.h"
#include "ProjectileManager.h"

class ProjectileEditor : Listener
{
public:
	ProjectileEditor();
	~ProjectileEditor() {}

	static inline std::shared_ptr<ProjectileEditor> CreateEditor() { return std::make_shared<ProjectileEditor>(); }
	void Initialise(const Graphics& gfx, ConstantBuffer<Matrices>& mat);
	void Update(const float dt);
	void Draw(ID3D11DeviceContext* context, XMMATRIX orthoMatrix);
	
	void SpawnEditorWindow(const Graphics& gfx, ConstantBuffer<Matrices>& mat);
	
	void AddToEvent() noexcept;
	void HandleEvent(Event* event) override;
	
private:
	void LoadPattern();
	void SavePattern();

	void ShowPattern();
	
	void TestButtons(const Graphics& gfx, ConstantBuffer<Matrices>& mat);
	
	void SpawnPattern();

	bool m_bSaveButton;
	std::string m_sSelectedFile;
	std::string m_sFilePath;
	std::string m_sFileContent;
	
	std::vector< ProjectileData::ProjectileJSON> m_vecProjectiles;
	std::shared_ptr<ProjectileManager> m_pProjectileManager;
};

#endif // !PROJECTILE_EDITOR_H
