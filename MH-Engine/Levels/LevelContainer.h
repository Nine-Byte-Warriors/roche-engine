#pragma once
#ifndef LEVELCONTAINER_H
#define LEVELCONTAINER_H

#include "Graphics.h"
#include "UIManager.h"

#if _DEBUG
#include "ImGuiManager.h"
#endif

/// <summary>
/// Loads and renders/updates all the components and models for the current scene/level.
/// Sets up any constant buffers that are specific to this particular scene/level.
/// Used to initialize objects native to every level.
/// </summary>
class LevelContainer
{
	friend class Application;
public:
	virtual ~LevelContainer( void ) = default;
#if _DEBUG
	inline void Initialize( Graphics* gfx, UIManager* ui, ImGuiManager* imgui )
	{
		m_ui = ui;
		m_gfx = gfx;
		m_imgui = imgui;
	}
#else
	inline void Initialize( Graphics* gfx, UIManager* ui )
	{
		m_ui = ui;
		m_gfx = gfx;
	}
#endif
	
	inline std::string GetLevelName() { return m_sLevelName; }
	inline Graphics* GetGraphics() const noexcept { return m_gfx; }
	inline UIManager* GetUIManager() const noexcept { return m_ui; }
#if _DEBUG
	inline ImGuiManager* GetImguiManager() const noexcept { return m_imgui; }
#endif

	// Render/Update Scene Functions
	virtual void OnCreate() {}
	virtual void OnSwitch() {}

	virtual void BeginFrame() {}
	virtual void RenderFrame() {}
	virtual void EndFrame_Start() {}
	virtual void EndFrame_End() {}

	virtual void Update( const float dt ) {}
	virtual void CleanUp() {}

protected:
	// Objects
	Graphics* m_gfx;
	UIManager* m_ui;
#if _DEBUG
	ImGuiManager* m_imgui;
#endif

	// Next Level
	std::string m_sLevelName;
	int m_iCurrentLevel;
	int m_iNextLevel;
};

#endif