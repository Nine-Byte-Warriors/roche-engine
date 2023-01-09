#pragma once
#ifndef TILEMAP_H
#define TILEMAP_H

#include <string>
#include <imgui/imgui.h>

#define ROWS 6
#define COLUMNS 6

enum TileType
{
	EMPTY,
	DIRT,
	WALL,
	SIZEOFTILETYPE //Should ALWAYS be the last value
};

struct TileTypeData
{
	TileType type;
	ImColor color;
	std::string name;
};

class TileMap
{
public:
	TileMap();

	void UpdateTile(int row, int column, TileType tileType);
	void UpdateTile(int pos, TileType tileType);

	void ResetTileMap();

	TileType* GetLevel();
	TileType GetTile(int pos);

	TileTypeData* GetTileTypeData();

private:
	const TileType m_EmptyLevel[ROWS * COLUMNS] =
	{
		EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,
		EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,
		EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,
		EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,
		EMPTY,EMPTY,EMPTY,EMPTY,EMPTY
	};

	TileType m_Level[ROWS * COLUMNS] =
	{
		EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,
		EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,
		EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,
		EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,
		EMPTY,EMPTY,EMPTY,EMPTY,EMPTY
	};

	TileTypeData m_sTileTypeData[SIZEOFTILETYPE];
};

#endif