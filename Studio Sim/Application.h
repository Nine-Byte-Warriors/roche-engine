#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

#include "Input.h"
#include "Timer.h"
#include "Level1.h"
#include "Graphics.h"
#include "WindowContainer.h"
#include "AudioEngine.h"

#if _DEBUG
#include "ImGuiManager.h"
#endif

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
#if _DEBUG
	ImGuiManager m_imgui;
#endif
	Graphics m_graphics;
	Timer m_timer;
	Input m_input;
};

#endif