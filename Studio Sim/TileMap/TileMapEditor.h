#pragma once
#ifndef TILEMAPEDITOR_H
#define TILEMAPEDITOR_H

#include "structures.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "DDSTextureLoader.h"
#include "TileMap.h"
#include <shobjidl.h> 
#include <imgui/imgui.h>

class TileMapEditor
{
public:
	TileMapEditor();

	void SpawnControlWindow();
private:
	bool OpenFileExplorer();
	void Load();
	bool LoadReadFile();
	void LoadProcessFile();
	void SaveToExistingFile();
	void SaveToNewFile();
	bool SaveWriteFile();

	void TileMapSelectionButtons();
	void TileMapSelectedText();
	void TileMapGridPreview();
	void UpdateTileMapGridPreview();

	bool StringEqualsIgnoreCase(const std::string& str1, const std::string& str2);

	TileMap tileMap;

	int m_iCurrentSelectedTileType;
	std::string m_sCurrentSelectedTileType;

	bool m_bLoadButton;
	std::string m_sSelectedFile = "LoadFile.txt";
	std::string m_sFilePath = "";
	std::string m_sFileContent = "";

	bool m_bSaveButton;
	bool m_bSaveNewButton;

	bool m_bTileMapPreviewImageButton[ROWS * COLUMNS];

	ImColor m_TileMapPreviewImageButtonColor[ROWS * COLUMNS];

	const ImVec2 m_vImageButtonSize = ImVec2(32, 32);
	const ImVec2 m_vImageButtonFrame0 = ImVec2(10, 10);
	const ImVec2 m_vImageButtonFrame1 = ImVec2(10, 10);
	const int m_iImageButtonPadding = 2;

	std::vector<TileTypeData> m_sTileTypeData;
	int m_iSizeOfTileTypeData;
};

#endif
