#pragma once
#ifndef TILEMAP_H
#define TILEMAP_H

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#if _DEBUG
#include <imgui/imgui.h>
#endif

//#define COLUMNS 40
//#define ROWS 22

enum class TileMapLayer
{
	Background,
	Foreground,
	Both
};

struct TileTypeData
{
	int type;
#if _DEBUG
	ImColor color;
	bool button;
#endif
	std::string name;
};

class TileMap
{
public:
	TileMap(int rows, int columns);

	void UpdateTile(int pos, int tileType);
	void ResetTileMap();

	int GetTileType(int pos);

	void SetRows(int rows);
	void SetColumns(int columns);

	std::vector<TileTypeData> GetTileTypeData();

private:

	std::vector<int> m_iLevel;

	std::vector<TileTypeData> m_sTileTypeData;

	std::string JsonFile = "Resources\\TileMaps\\TileMap.json";
	json TileMapJsonData;

	int m_iRows;
	int m_iColumns;
};

#endif
