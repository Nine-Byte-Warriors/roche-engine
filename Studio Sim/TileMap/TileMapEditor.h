#pragma once
#ifndef TILEMAPEDITOR_H
#define TILEMAPEDITOR_H

#include "Shader_Data.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "DDSTextureLoader.h"
#include "TileMap.h"
#include <shobjidl.h> 

#if _DEBUG
#include <imgui/imgui.h>
#endif

class TileMapEditor
{
public:
	TileMapEditor();

#if _DEBUG
	void SpawnControlWindow();
#endif

	bool UpdateDrawOnceAvalible();
	bool UpdateDrawContinuousAvalible();
	void UpdateDrawOnceDone();

	std::string GetTileTypeName(int pos);
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
  
	void DrawButton();
	TileMap tileMap;

	int m_iCurrentSelectedTileType;
	std::string m_sCurrentSelectedTileType;

	bool m_bLoadButton;
	std::string m_sSelectedFile = "LoadFile.txt";
	std::string m_sFilePath = "";
	std::string m_sFileContent = "";

	bool m_bSaveButton;
	bool m_bSaveNewButton;

	bool m_bDrawButton;
	bool m_bDrawOnce;
	bool m_bDrawContinuous;

	bool m_bTileMapPreviewImageButton[COLUMNS * ROWS];

#if _DEBUG
	ImColor m_TileMapPreviewImageButtonColor[COLUMNS * ROWS];

	const ImVec2 m_vImageButtonSize = ImVec2(32, 32);
	const ImVec2 m_vImageButtonFrame0 = ImVec2(10, 10);
	const ImVec2 m_vImageButtonFrame1 = ImVec2(10, 10);
#endif
	const int m_iImageButtonPadding = 2;

	std::vector<TileTypeData> m_sTileTypeData;
	int m_iSizeOfTileTypeData;
};

#endif
