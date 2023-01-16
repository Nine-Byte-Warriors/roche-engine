#include "stdafx.h"
#include "TileMap.h"

TileMap::TileMap(int rows, int columns)
{
	m_iRows = rows;
	m_iColumns = columns;

	std::ifstream TileMapJsonFile(JsonFile);
	json TileMapJsonData = json::parse(TileMapJsonFile);
	int pos = 0;
	int type = 0;
	for (auto& tiles : TileMapJsonData.items())
	{
		TileTypeData tile;
		tile.type = type;
		tile.name = tiles.value().at("name");
#if _DEBUG
		tile.button = false;
		int colorA = tiles.value().at("colorA");
		tile.color = ImColor(tiles.value().at("colorR"), tiles.value().at("colorG"), tiles.value().at("colorB"), colorA);
#endif

		pos++;
		type++;
		m_sTileTypeData.push_back(tile);
	}

	ResetTileMap();
}

void TileMap::UpdateTile(int pos, int tileType)
{
	m_iLevel[pos] = tileType;
}

void TileMap::ResetTileMap()
{
	for (int i = 0; i < m_iRows * m_iColumns; i++)
	{
		m_iLevel.push_back(m_sTileTypeData[0].type);
	}
}

int TileMap::GetTileType(int pos)
{
	return m_iLevel[pos];
}

std::vector<TileTypeData> TileMap::GetTileTypeData()
{
	return m_sTileTypeData;
}
