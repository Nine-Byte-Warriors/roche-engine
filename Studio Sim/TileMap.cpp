#include "stdafx.h"
#include "TileMap.h"

TileMap::TileMap()
{
	m_sTileTypeData[0].type = EMPTY;
	m_sTileTypeData[1].type = DIRT;
	m_sTileTypeData[2].type = WALL;

	m_sTileTypeData[0].color = ImColor(0, 0, 0, 255);
	m_sTileTypeData[1].color = ImColor(155, 118, 83, 255);
	m_sTileTypeData[2].color = ImColor(220, 40, 40, 255);

	m_sTileTypeData[0].name = "EMPTY";
	m_sTileTypeData[1].name = "DIRT";
	m_sTileTypeData[2].name = "WALL";
}

void TileMap::UpdateTile(int row, int column, TileType tileType)
{
	int levelPos = row * ROWS + column;

	m_Level[levelPos] = tileType;
}

void TileMap::UpdateTile(int pos, TileType tileType)
{
	m_Level[pos] = tileType;
}

void TileMap::ResetTileMap()
{
	for (int i = 0; i < ROWS * COLUMNS; i++)
	{
		m_Level[i] = EMPTY;
	}	
}

TileType* TileMap::GetLevel()
{
	return m_Level;
}

TileType TileMap::GetTile(int pos)
{
	return m_Level[pos];
}

TileTypeData* TileMap::GetTileTypeData()
{
	return m_sTileTypeData;
}
