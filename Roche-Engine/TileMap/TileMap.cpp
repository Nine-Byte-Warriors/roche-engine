#include "stdafx.h"
#include "TileMap.h"

TileMap::TileMap(int rows, int columns)
{
	m_iRows = rows;
	m_iColumns = columns;

	JsonLoading::LoadJson(m_sTileTypeData, JsonFile);

	for (int i = 0; i < m_sTileTypeData.size(); i++)
	{
		m_sTileTypeData[i].type = i;
	}

	ResetTileMap();
}

void TileMap::UpdateTileFromType(int pos, int tileType)
{
	m_iLevel[pos] = tileType;
}

void TileMap::UpdateTileFromName(int pos, std::string name)
{
	for (int i = 0; i < m_sTileTypeData.size(); i++)
	{
		if (m_sTileTypeData[i].name == name)
		{
			m_iLevel[pos] = m_sTileTypeData[i].type;
		}
	}
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

std::vector<int> TileMap::GetLevel()
{
	return m_iLevel;
}

std::string TileMap::GetTileTypeNameFromType(int type)
{
	return m_sTileTypeData[type].name;
}

std::string TileMap::GetTileTypeNameFromPosition(int pos)
{
	return m_sTileTypeData[m_iLevel[pos]].name;
}

std::vector<TileTypeData> TileMap::GetTileTypeData()
{
	return m_sTileTypeData;
}

#if _DEBUG
ImColor TileMap::GetColorFromType(int pos)
{
	return ImColor(m_sTileTypeData[pos].colorR, m_sTileTypeData[pos].colorG, m_sTileTypeData[pos].colorB, m_sTileTypeData[pos].colorA);
}

ImColor TileMap::GetColorFromPosition(int pos)
{
	return ImColor(m_sTileTypeData[m_iLevel[pos]].colorR, m_sTileTypeData[m_iLevel[pos]].colorG, m_sTileTypeData[m_iLevel[pos]].colorB, m_sTileTypeData[m_iLevel[pos]].colorA);
}
#endif
