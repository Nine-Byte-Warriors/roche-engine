#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

#include "Input.h"
#include "Timer.h"
#include "Level.h"
#include "Graphics.h"
#include "UIManager.h"
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
	int m_uCurrLevelId = 1;
	LevelStateMachine m_stateMachine;
	std::vector<uint32_t> m_uLevel_IDs;
	std::vector<std::shared_ptr<Level>> m_pLevels;

	// Objects
#if _DEBUG
	ImGuiManager m_imgui;
#endif
	UIManager m_uiManager;
	Graphics m_graphics;
	Timer m_timer;
	Input m_input;
};

#endif