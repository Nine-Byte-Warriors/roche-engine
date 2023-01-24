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

struct LevelData
{
	std::string name;
	std::string audio;
	std::string entity;
	std::string tileMap;
	std::string ui;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( LevelData, name, audio, entity, tileMap, ui )

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
	std::string m_sAudioFile;
	std::string m_sEntityFile;
	std::string m_sTileMapFile;
	std::string m_sUIFile;

	std::string m_sFilePath;
	std::vector<LevelData> m_vLevelData;
	std::string m_sJsonFile = "Levels.json";

	int m_iCurrLevelId = -1;
	int m_iActiveLevelIdx = 0;
	bool m_bFirstLoad = false;
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