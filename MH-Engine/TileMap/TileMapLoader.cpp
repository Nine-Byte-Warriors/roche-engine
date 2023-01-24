#include "stdafx.h"
#include "TileMapLoader.h"

TileMapLoader::TileMapLoader(int rows, int columns)
{
	m_iRows = rows;
	m_iColumns = columns;

	m_tileMapBackground = new TileMap(m_iRows, m_iColumns);
	m_tileMapForeground = new TileMap(m_iRows, m_iColumns);
}

TileMapLoader::~TileMapLoader()
{
	delete m_tileMapBackground;
	delete m_tileMapForeground;
}

void TileMapLoader::SetLevel(TileMap *background, TileMap *foreground)
{ 
	m_tileMapBackground = background;
	m_tileMapForeground = foreground;
	m_sTileTypeData = m_tileMapBackground->GetTileTypeData();
}

void TileMapLoader::LoadLevel(std::string filePathBackground, std::string filePathForeground)
{
	std::vector<std::string> tileTypeName;
	JsonLoading::LoadJson(tileTypeName, filePathBackground);

	for (int i = 0; i < m_iRows * m_iColumns; i++)
	{
		m_tileMapBackground->UpdateTileFromName(i, tileTypeName[i]);
	}

	tileTypeName.clear();
	JsonLoading::LoadJson(tileTypeName, filePathForeground);

	for (int i = 0; i < m_iRows * m_iColumns; i++)
	{
		m_tileMapForeground->UpdateTileFromName(i, tileTypeName[i]);
	}

	m_sTileTypeData = m_tileMapBackground->GetTileTypeData();
}

std::string TileMapLoader::GetTileTypeName(int pos, TileMapLayer tileMapLayer)
{
	if (tileMapLayer == TileMapLayer::Background)
	{
		return m_sTileTypeData[m_tileMapBackground->GetTileType(pos)].name;
	}
	else if (tileMapLayer == TileMapLayer::Foreground)
	{
		return m_sTileTypeData[m_tileMapForeground->GetTileType(pos)].name;
	}
}
