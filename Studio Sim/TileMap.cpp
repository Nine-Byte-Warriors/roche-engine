#include "stdafx.h"
#include "TileMap.h"

TileMap::TileMap()
{

}

void TileMap::UpdateTile(int row, int column, TileType tileType)
{
	int levelPos = row * ROWS + column;

	m_iLevel[levelPos] = tileType;
}

void TileMap::UpdateTile(int pos, TileType tileType)
{
	m_iLevel[pos] = tileType;
}

void TileMap::ResetTileMap()
{
	for (int i = 0; i < ROWS * COLUMNS; i++)
	{
		m_iLevel[i] = EMPTY;
	}	
}

TileType* TileMap::GetLevel()
{
	return m_iLevel;
}

TileType TileMap::GetTile(int pos)
{
	return m_iLevel[pos];
}
