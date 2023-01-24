#include "stdafx.h"
#include "TileMapLoader.h"

#define FOLDER_PATH "Resources\\TileMaps\\"

TileMapLoader::TileMapLoader() { }

TileMapLoader::~TileMapLoader() { }

void TileMapLoader::Initialize(int rows, int columns)
{
	m_iRows = rows;
	m_iColumns = columns;

	m_tileMapBackground = std::make_shared<TileMap>(m_iRows, m_iColumns);
	m_tileMapForeground = std::make_shared<TileMap>(m_iRows, m_iColumns);
}

void TileMapLoader::SetLevel(std::shared_ptr<TileMap> background, std::shared_ptr<TileMap> foreground)
{ 
	m_tileMapBackground = background;
	m_tileMapForeground = foreground;
	m_sTileTypeData = m_tileMapBackground->GetTileTypeData();
}

void TileMapLoader::LoadLevel(std::string filePathBackground, std::string filePathForeground)
{
	std::vector<std::string> tileTypeName;
	JsonLoading::LoadJson(tileTypeName, FOLDER_PATH + filePathBackground);

	for (int i = 0; i < m_iRows * m_iColumns; i++)
	{
		m_tileMapBackground->UpdateTileFromName(i, tileTypeName[i]);
	}

	tileTypeName.clear();
	JsonLoading::LoadJson(tileTypeName, FOLDER_PATH + filePathForeground);

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
