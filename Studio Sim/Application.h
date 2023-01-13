#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

#include <dxtk/SpriteFont.h>
#include <dxtk/SpriteBatch.h>
#include "DDSTextureLoader.h"
#include "Resource.h"

#include "Input.h"
#include "Timer.h"
#include "Shaders.h"
#include "structures.h"
#include "ImGuiManager.h"
#include "ConstantBuffer.h"
#include "WindowContainer.h"
#include "Graphics.h"
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
	uint32_t m_uLevel1_ID;
	LevelStateMachine m_stateMachine;
	std::shared_ptr<Level1> m_pLevel1;

	// Objects
	ImGuiManager m_imgui;
	Graphics m_graphics;
	Timer m_timer;
	Input m_input;
};

#endif