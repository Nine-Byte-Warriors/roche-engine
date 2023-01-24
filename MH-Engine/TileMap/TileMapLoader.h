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
	TileMapLoader(int rows, int columns);
	~TileMapLoader();

	void SetLevel(TileMap *background, TileMap *foreground);

	void LoadLevel(std::string filePathBackground, std::string filePathForeground);

	std::string GetTileTypeName(int pos, TileMapLayer tileMapLayer);

private:
	int m_iRows;
	int m_iColumns;
	int m_iLevel;

	TileMap* m_tileMapBackground;
	TileMap* m_tileMapForeground;

	TileMapLayer m_tileMapLayer;

	std::vector<TileTypeData> m_sTileTypeData;
};



#endif
