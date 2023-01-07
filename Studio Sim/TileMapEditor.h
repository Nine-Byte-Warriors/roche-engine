#pragma once
#include "structures.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "DDSTextureLoader.h"
#include <shobjidl.h> 
#include <TileMap.h>
#include <imgui/imgui.h>

class TileMapEditor
{
public:
	TileMapEditor();

	void SpawnControlWindow();

	void Load();
	bool OpenFileExplorer();
	bool ReadFile();
	void ProcessFile();

	void SaveToExistingFile();
	void SaveToNewFile();
	bool WriteFile();

	void TileMapSelectionButtons();
	void TileMapSelectedText();
	void TileMapGridPreview();
	void UpdateTileMapGridPreview();
private:
	TileMap tileMap;

	int m_iCurrentSelectedTileType;
	std::string m_sCurrentSelectedTileType;

	bool m_bLoadButton;
	std::string m_sSelectedFile = "LoadFile.txt";
	std::string m_sFilePath = "";
	std::string m_sFileContent = "";

	bool m_bSaveButton;
	bool m_bSaveNewButton;

	bool m_bEmptyImageButton;
	bool m_bDirtImageButton;
	bool m_bWallImageButton;
	bool m_bTileMapPreviewImageButton[ROWS * COLUMNS];

	ImColor m_EmptyImageButtonColor;
	ImColor m_DirtImageButtonColor;
	ImColor m_WallImageButtonColor;
	ImColor m_TileMapPreviewImageButtonColor[ROWS * COLUMNS];

	const ImVec2 m_vImageButtonSize = ImVec2(32, 32);
	const ImVec2 m_vImageButtonFrame0 = ImVec2(10, 10);
	const ImVec2 m_vImageButtonFrame1 = ImVec2(10, 10);
	const int m_iImageButtonPadding = 2;
};

