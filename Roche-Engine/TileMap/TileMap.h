#pragma once
#ifndef TILEMAP_H
#define TILEMAP_H

#include <nlohmann/json.hpp>
using json = nlohmann::json;
#include "JsonLoading.h"

//#define COLUMNS 40
//#define ROWS 22

enum class TileMapLayer
{
	Background,
	Foreground
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
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TileTypeData, name, colorR, colorG, colorB, colorA, button)
#else
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TileTypeData, name, button)
#endif

class TileMap
{
public:
	TileMap(int rows, int columns);

	void UpdateTileFromType(int pos, int tileType);
	void UpdateTileFromName(int pos, std::string name);
	void ResetTileMap();

	int GetTileType(int pos);
	std::string GetTileTypeNameFromType(int type);
	std::string GetTileTypeNameFromPosition(int pos);
	std::vector<int> GetLevel();

	std::vector<TileTypeData> GetTileTypeData();

#if _DEBUG
	ImColor GetColorFromType(int pos);
	ImColor GetColorFromPosition(int pos);
#endif

private:
	std::vector<int> m_iLevel;

	std::vector<TileTypeData> m_sTileTypeData;

	std::string JsonFile = "Resources\\TileMaps\\TileMap.json";
	json TileMapJsonData;

	int m_iRows;
	int m_iColumns;
};

#endif
