#pragma once
#ifndef LEVELCONTAINER_H
#define LEVELCONTAINER_H

#include "Graphics.h"

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
	inline void Initialize( Graphics* gfx, ImGuiManager* imgui )
	{
		graphics = gfx;
		m_imgui = imgui;
	}
#else
	inline void Initialize( Graphics* gfx )
	{
		graphics = gfx;
	}
#endif
	
	inline std::string GetLevelName() { return levelName; }
	inline Graphics* GetGraphics() const noexcept { return graphics; }

	// Render/Update Scene Functions
	virtual void OnCreate() {}
	virtual void OnSwitch() {}

	virtual void BeginFrame() {}
	virtual void RenderFrame() {}
	virtual void EndFrame() {}

	virtual void Update( const float dt ) {}
	virtual void CleanUp() {}

protected:
	// Objects
	Graphics* graphics;
#if _DEBUG
	ImGuiManager* m_imgui;
#endif

	// Next Level
	std::string levelName;
	UINT32 NextLevel;
	UINT32 CurrentLevel;
};

#endif