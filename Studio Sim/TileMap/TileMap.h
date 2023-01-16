#pragma once
#ifndef TILEMAP_H
#define TILEMAP_H

#include <nlohmann/json.hpp>
using json = nlohmann::json;
#include "JsonLoading.h"

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
	std::string name;
	int type;
#if _DEBUG
	int colorR;
	int colorG;
	int colorB;
	int colorA;
#endif
	bool button;
};
#if _DEBUG
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TileTypeData, name, type, colorR, colorG, colorB, colorA, button)
#else
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TileTypeData, name, type, button)
#endif

class TileMap
{
public:
	TileMap(int rows, int columns);

	void UpdateTile(int pos, int tileType);
	void ResetTileMap();

	int GetTileType(int pos);

	std::vector<TileTypeData> GetTileTypeData();

	ImColor GetColor(int pos) { return ImColor(m_sTileTypeData[pos].colorR, m_sTileTypeData[pos].colorG, m_sTileTypeData[pos].colorB, m_sTileTypeData[pos].colorA); };

private:

	std::vector<int> m_iLevel;

	std::vector<TileTypeData> m_sTileTypeData;

	std::string JsonFile = "Resources\\TileMaps\\TileMap.json";
	json TileMapJsonData;

	int m_iRows;
	int m_iColumns;
};

#endif
