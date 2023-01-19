#pragma once
#ifndef UIEDITOR_H
#define UIEDITOR_H

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

	void LoadFromFile();
#if _DEBUG
	void SaveToFile();
	void SpawnControlWindow( int width, int height );
#endif
private:
	std::string m_sFilePath;
	std::string m_sFileContent;
	std::string m_sSelectedFile;

	std::string m_sJsonFile = "Resources\\UI\\UIScreens.json";
	std::vector<std::string> m_vUITypes = { "Colour", "Button", "Image", "Data Slider", "Page Slider", "Energy Bar", "Input" };
	std::map<std::string, std::vector<UIScreenData>> m_vUIScreenData; // list of UI components for a given screen
};

#endif