#pragma once
#include <string>
#define ROWS 5
#define COLUMNS 5

enum TileType
{
	EMPTY,
	DIRT,
	WALL
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
	

private:
	const TileType m_iEmptyLevel[ROWS * COLUMNS] =
	{
		EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,
		EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,
		EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,
		EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,
		EMPTY,EMPTY,EMPTY,EMPTY,EMPTY
	};

	TileType m_iLevel[ROWS * COLUMNS] =
	{
		EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,
		EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,
		EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,
		EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,
		EMPTY,EMPTY,EMPTY,EMPTY,EMPTY
	};
};

