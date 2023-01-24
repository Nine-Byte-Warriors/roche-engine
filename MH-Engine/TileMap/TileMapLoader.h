#pragma once
#ifndef TILEMAPLOADER_H
#define TILEMAPLOADER_H

#include <nlohmann/json.hpp>
using json = nlohmann::json;
#include "JsonLoading.h"
#include "TileMap.h"

#if _DEBUG
#include <imgui/imgui.h>
#endif

//#define COLUMNS 40
//#define ROWS 22

class TileMapLoader
{
public:
	TileMapLoader();
	~TileMapLoader();

	void Initialize(int rows, int columns);
	void SetLevel(std::shared_ptr<TileMap> background, std::shared_ptr<TileMap> foreground);
	void LoadLevel(std::string filePathBackground, std::string filePathForeground);
	std::string GetTileTypeName(int pos, TileMapLayer tileMapLayer);

private:
	int m_iRows;
	int m_iColumns;
	int m_iLevel;

	std::shared_ptr<TileMap> m_tileMapBackground;
	std::shared_ptr<TileMap> m_tileMapForeground;

	TileMapLayer m_tileMapLayer;

	std::vector<TileTypeData> m_sTileTypeData;
};



#endif
