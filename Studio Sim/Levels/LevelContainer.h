#pragma once
#ifndef LEVELCONTAINER_H
#define LEVELCONTAINER_H

#include "Camera.h"
#include "Graphics.h"
#include "ImGuiManager.h"

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
	void Initialize( Graphics* gfx, Camera* camera, ImGuiManager* imgui )
	{
		graphics = gfx;
		m_camera = camera;
		m_imgui = imgui;
	}
	
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
	Camera* m_camera;
	Graphics* graphics;
	ImGuiManager* m_imgui;

	// Next Level
	std::string levelName;
	UINT32 NextLevel;
	UINT32 CurrentLevel;
};

#endif