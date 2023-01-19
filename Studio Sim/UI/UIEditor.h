#pragma once
#ifndef UIEDITOR_H
#define UIEDITOR_H

class Graphics;
#include "UIScreen.h"
#include "JsonLoading.h"

struct UIScreenList
{
	std::string name;
	std::string file;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( UIScreenList, name, file )

struct UIScreenData
{
	std::string name;
	std::string type;
	std::vector<float> position;
	std::vector<float> scale;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( UIScreenData, name, type, position, scale )

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
private:
	std::string m_sFilePath;
	std::string m_sFileContent;
	std::string m_sSelectedFile;

	std::string m_sJsonFile = "Main Menu.json";
	std::vector<std::string> m_vUITypes = { "Colour", "Button", "Image", "Data Slider", "Page Slider", "Energy Bar", "Input" };
	
	std::vector<UIScreenList> m_vUIScreenList; // list of UI screens
	std::map<std::string, std::vector<UIScreenData>> m_vUIScreenData; // list of UI components for a given screen
};

#endif