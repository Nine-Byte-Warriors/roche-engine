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
	bool hide;
	int zindex;
	std::string name;
	std::string type;
	std::string action;
	std::vector<float> position;
	std::vector<float> scale;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( UIWidgetData, hide, zindex, name, type, action, position, scale )

class UIEditor
{
public:
	UIEditor();
	~UIEditor();

	void SetJsonFile( const std::string& name );
	void LoadFromFile_Screens();
	void LoadFromFile_Widgets();

#if _DEBUG
	void SaveToFile_Screens();
	void SaveToFile_Widgets();
	void Update( const float dt );
	void SpawnControlWindow( const Graphics& gfx );
#endif

	inline bool GetShouldUpdate() const noexcept { return m_bRequiresUpdate; }
	inline void SetShouldUpdate( bool update ) noexcept { m_bRequiresUpdate = update; }

	inline bool ShouldShowAll() const noexcept { return m_bShouldShowAll; }
	inline bool ShouldHideAll() const noexcept { return m_bShouldHideAll; }

	inline int GetCurrentScreenIndex() const noexcept { return m_iCurrentScreenIdx; }
	inline std::string GetCurrentScreenName() const noexcept { return m_vUIScreenData[m_iCurrentScreenIdx].name; }

	inline const std::vector<UIScreenData>& GetScreenData() const noexcept { return m_vUIScreenData; }
	inline const std::map<std::string, std::vector<UIWidgetData>>& GetWidgetData() const noexcept { return m_vUIWidgetData; }

	inline const std::vector<std::vector<std::shared_ptr<Widget>>>& GetWidgets() const noexcept { return m_vUIWidgets; }
	inline const std::vector<std::shared_ptr<UIScreen>>& GetScreens() const noexcept { return m_vUIScreens; }

private:
	void SortScreens();
	void CreateScreens();
	void CreateWidgets();

	std::string m_sFilePath;
	std::string m_sFileContent;
	std::string m_sSelectedFile;
	int m_iCurrentScreenIdx = -1;
	bool m_bShouldShowAll = false;
	bool m_bShouldHideAll = false;
	bool m_bRequiresUpdate = true;

	std::string m_sJsonFile = "Menu.json";
	const std::vector<std::string> m_vUITypes =
	{
		"Button", "Colour Block",
		"Data Slider", "Drop Down",
		"Energy Bar", "Image",
		"Input", "Page Slider"
	};

	std::vector<std::vector<std::shared_ptr<Widget>>> m_vUIWidgets; // list of widgets per screen
	std::vector<std::shared_ptr<UIScreen>> m_vUIScreens; // list of UI screen objects

	std::vector<UIScreenData> m_vUIScreenData; // list of UI screen data
	std::map<std::string, std::vector<UIWidgetData>> m_vUIWidgetData; // list of UI components for a given screen
};

#endif