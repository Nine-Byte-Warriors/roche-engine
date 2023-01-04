#include "stdafx.h"
#include "TileMap.h"

TileMap::TileMap(std::string inputFile)
{

}

TileMap::TileMap()
{
}

void TileMap::UpdateTile(int row, int column, TileType tileType)
{
	int levelPos = row * ROWS + column;

	m_iLevel[levelPos] = tileType;
}

void TileMap::ResetTileMap()
{
	for (int i = 0; i < ROWS * COLUMNS; i++)
	{
		m_iLevel[i] = EMPTY;
	}	
}

int* TileMap::GetLevel()
{
	return m_iLevel;
}
