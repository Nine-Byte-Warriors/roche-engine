#pragma once
#ifndef TILEMAP_H
#define TILEMAP_H

#include <string>
#include <imgui/imgui.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#define COLUMNS 6
#define ROWS 8

struct TileTypeData
{
	int type;
	ImColor color;
	bool button;
	std::string name;
};

class TileMap
{
public:
	TileMap();

	void UpdateTile(int row, int column, int tileType);
	void UpdateTile(int pos, int tileType);

	void ResetTileMap();

	int* GetLevel();
	int GetTileType(int pos);

	std::vector<TileTypeData> GetTileTypeData();

private:

	int m_Level[COLUMNS * ROWS];

	std::vector<TileTypeData> m_sTileTypeData;

	std::string JsonFile = "Resources\\TileMaps\\TileMap.json";
	json TileMapJsonData;
};

#endif
