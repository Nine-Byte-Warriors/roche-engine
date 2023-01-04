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
//#include "MainMenu.h"
//#include "CreditsMenu.h"

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
	//uint32_t credits_ID;
	//uint32_t mainMenu_ID;

	LevelStateMachine stateMachine;
	std::shared_ptr<Level1> level1;
	//std::shared_ptr<Credits_Level> Credits;
	//std::shared_ptr<MainMenu_Level> MainMenu;

	// Objects
	Camera m_camera;
	ImGuiManager m_imgui;

	// Data
	Timer m_timer;
	Input m_input;
};

#endif