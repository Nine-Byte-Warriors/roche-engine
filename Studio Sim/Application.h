#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

#include <dxtk/SpriteFont.h>
#include <dxtk/SpriteBatch.h>
#include "DDSTextureLoader.h"
#include "Resource.h"

#include "Cube.h"
#include "Input.h"
#include "Timer.h"
#include "Shaders.h"
#include "structures.h"
#include "ImGuiManager.h"
#include "ConstantBuffer.h"
#include "PostProcessing.h"
#include "WindowContainer.h"
#include "Level1.h"

class Application : public WindowContainer
{
public:
	bool Initialize( HINSTANCE hInstance, int width, int height );
	void CleanupDevice();

	bool ProcessMessages() noexcept;
	void Update();
	void Render();
private:
	// Levels
	uint32_t level1_ID;
	std::shared_ptr<Level1> level1;
	LevelStateMachine stateMachine;

	// Objects
	Camera m_camera;
	ImGuiManager m_imgui;

	// Data
	Timer m_timer;
	Input m_input;
};

#endif