#include "stdafx.h"
#include "TileMap.h"

TileMap::TileMap()
{
	std::ifstream TileMapJsonFile(JsonFile);
	json TileMapJsonData = json::parse(TileMapJsonFile);
	int pos = 0;
	for (auto& tiles : TileMapJsonData.items())
	{
		TileTypeData tile;
		tile.type = tiles.value().at("type");
		tile.name = tiles.value().at("name");
		tile.button = false;
		int colorA = tiles.value().at("colorA");
		tile.color = ImColor(tiles.value().at("colorR"), tiles.value().at("colorG"), tiles.value().at("colorB"), colorA);

		pos++;
		m_sTileTypeData.push_back(tile);
	}

	ResetTileMap();
}

void TileMap::UpdateTile(int row, int column, int tileType)
{
	int levelPos = row * ROWS + column;

	m_Level[levelPos] = tileType;
}

void TileMap::UpdateTile(int pos, int tileType)
{
	m_Level[pos] = tileType;
}

void TileMap::ResetTileMap()
{
	for (int i = 0; i < ROWS * COLUMNS; i++)
	{
		m_Level[i] = m_sTileTypeData[0].type;
	}
}

int* TileMap::GetLevel()
{
	return m_Level;
}

int TileMap::GetTileType(int pos)
{
	return m_Level[pos];
}

std::vector<TileTypeData> TileMap::GetTileTypeData()
{
	return m_sTileTypeData;
}