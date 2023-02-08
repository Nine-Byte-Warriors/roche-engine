#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

#include "Input.h"
#include "Level.h"
#include "Graphics.h"
#include "UIManager.h"
#include "WindowContainer.h"
#include "AudioEngine.h"

#if _DEBUG
#include "ImGuiManager.h"
#endif

#define STARTING_LEVEL_NAME "Menu"

struct LevelData
{
	std::string name;
	std::string audio;
	std::string entity;
	std::string tmBack;
	std::string tmFront;
	std::string ui;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( LevelData, name, audio, entity, tmBack, tmFront, ui )

class Application : public WindowContainer, public Listener
{
public:
	bool Initialize( HINSTANCE hInstance, int width, int height );
	void CleanupDevice();

	bool ProcessMessages() noexcept;
	void Update();
	void Render();
private:
	void AddToEvent() noexcept;
	void RemoveFromEvent() noexcept;
	void HandleEvent(Event* event) override;

	void AddLevel(std::string levelName);
	void RemoveLevel(std::string levelName);

	// Levels
	std::string m_sAudioFile;
	std::string m_sEntityFile;
	std::string m_sTileMapBackFile;
	std::string m_sTileMapFrontFile;
	std::string m_sUIFile;

	std::string m_sFilePath;
	std::vector<LevelData> m_vLevelData;
	std::string m_sJsonFile = "Levels.json";

	std::string m_sCurrentLevelName = "Menu";
	int m_iActiveLevelIdx = 0;
	bool m_bFirstLoad = false;
	LevelStateMachine m_stateMachine;
	std::vector<std::string> m_sLevelNames;
	//std::vector<uint32_t> m_uLevel_IDs;
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