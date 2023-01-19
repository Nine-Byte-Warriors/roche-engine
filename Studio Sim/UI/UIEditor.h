#pragma once
#ifndef UIEDITOR_H
#define UIEDITOR_H

class Graphics;
#include "UIScreen.h"
#include "JsonLoading.h"

struct UIScreenData
{
	std::string name;
	std::string file;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( UIScreenData, name, file )

struct UIWidgetData
{
	std::string name;
	std::string type;
	std::vector<float> position;
	std::vector<float> scale;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( UIWidgetData, name, type, position, scale )

class UIEditor
{
public:
	UIEditor();
	~UIEditor();

	void LoadFromFile_Screens();
	void LoadFromFile_Widgets();
	void SortScreens();

#if _DEBUG
	void SaveToFile_Screens();
	void SaveToFile_Widgets();
	void SpawnControlWindow( const Graphics& gfx );
#endif
	
	inline std::vector<UIScreenData> GetScreenData() const noexcept { return m_vUIScreenData; }
	inline std::map<std::string, std::vector<UIWidgetData>> GetWidgetData() const noexcept { return m_vUIWidgetData; }

	inline std::vector<std::vector<Widget>> GetWidgets() const noexcept { return m_vUIWidgets; }
	inline std::vector<std::shared_ptr<UIScreen>> GetScreens() const noexcept { return m_vUIScreens; }

private:
	std::string m_sFilePath;
	std::string m_sFileContent;
	std::string m_sSelectedFile;

	std::string m_sJsonFile = "Main Menu.json";
	const std::vector<std::string> m_vUITypes =
	{
		"Button", "Colour",
		"Data Slider", "Energy Bar",
		"Image", "Input", "Page Slider"
	};
	
	std::vector<std::vector<Widget>> m_vUIWidgets; // list of widgets per screen
	std::vector<std::shared_ptr<UIScreen>> m_vUIScreens; // list of UI screen objects

	std::vector<UIScreenData> m_vUIScreenData; // list of UI screen data
	std::map<std::string, std::vector<UIWidgetData>> m_vUIWidgetData; // list of UI components for a given screen
};

#endif