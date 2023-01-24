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
	TileMapEditor(int rows, int columns);
	~TileMapEditor();

#if _DEBUG
	void SpawnControlWindow();
#endif

	bool IsDrawOnceAvalible();
	bool IsDrawContinuousAvalible();
	bool IsLayerSwitched();
	bool IsLoadedFile();

	void SetDrawOnceDone();
	void SetMapDrawnDone();
	void SetLayerSwitchedDone();
	void SetLoadedFileDone();

	std::vector<int> GetUpdatedTileMapTiles();
	void SetClearUpdatedTileMapTiles();

	TileMap* GetLevel(TileMapLayer layer);

	TileMapLayer GetTileMapLayer();
	int GetTileMapLayerInt();
	int GetTileMapOtherLayerInt();
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

	TileMap* m_tileMapBackground;
	TileMap* m_tileMapForeground;

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
	bool m_bLayerSwitched;
	bool m_bLoadedFile;

	std::vector<bool> m_bTileMapPreviewImageButton;

	int m_iRows;
	int m_iColumns;

	std::vector<int> m_iUpdatedTileMapTiles;

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
