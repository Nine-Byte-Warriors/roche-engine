#pragma once
#ifndef TILEMAPEDITOR_H
#define TILEMAPEDITOR_H

#include "Shader_Data.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "DDSTextureLoader.h"
#include "TileMap.h"

#if _DEBUG
#include <imgui/imgui.h>
#endif

class TileMapEditor
{
public:
	TileMapEditor();
	~TileMapEditor();

	void Initialize(int rows, int columns);
	void SetJsonFile( const std::string& name );

#if _DEBUG
	void SpawnControlWindow();
#endif

	bool UpdateDrawOnceAvalible();
	bool UpdateDrawContinuousAvalible();
	void UpdateDrawOnceDone();
	void UpdateMapDrawn();

	std::shared_ptr<TileMap> GetLevel(TileMapLayer layer);

	TileMapLayer GetTileMapLayer();
private:
	void Load();
	bool LoadProcessFile();
	void SaveToExistingFile();
	void SaveToNewFile();
	bool SaveWriteFile();

	void TileMapSelectionButtons();
	void TileMapSelectedText();
	void TileMapGridPreview();
	void TileMapGridInit();
	void UpdateSingleTileMapGridPreview();
	void UpdateWholeTileMapGridPreview();
  
	void DrawButton();

	void SelectTileMapLayer();

	std::shared_ptr<TileMap> m_tileMapBackground;
	std::shared_ptr<TileMap> m_tileMapForeground;

	int m_iTileMapLayer = 0;
	char m_cSaveFileName[128] = "";
	bool m_bInitTileGrid = false;
	std::string m_sPreviewMapLayer = "Background";

	int m_iCurrentSelectedTileType;
	std::string m_sCurrentSelectedTileType;

	std::string m_sSelectedFile = "LoadFile.txt";
	std::string m_sFilePath = "";
	std::string m_sFileContent = "";

	bool m_bSaveButton;
	bool m_bSaveNewButton;

	bool m_bDrawButton;
	bool m_bDrawOnce;
	bool m_bDrawContinuous;
	bool m_bMapUpdated;

	std::vector<bool> m_bTileMapPreviewImageButton;

	int m_iRows;
	int m_iColumns;

#if _DEBUG
	std::vector<ImColor> m_TileMapPreviewImageButtonColor;

	const ImVec2 m_vImageButtonSize = ImVec2(5, 10);
	const ImVec2 m_vImageButtonFrame0 = ImVec2(5, 5);
	const ImVec2 m_vImageButtonFrame1 = ImVec2(5, 5);
#endif
	const int m_iImageButtonPadding = 1;

	std::vector<TileTypeData> m_sTileTypeData;
	int m_iSizeOfTileTypeData;

	TileMapLayer m_tileMapLayer;
};

#endif
