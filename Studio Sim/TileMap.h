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
private:

	const int m_iEmptyLevel[ROWS * COLUMNS] =
	{
		EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,
		EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,
		EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,
		EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,
		EMPTY,EMPTY,EMPTY,EMPTY,EMPTY
	};
	
	int m_iLevel[ROWS * COLUMNS] =
	{
		WALL,WALL,WALL,WALL,WALL,
		WALL,DIRT,DIRT,DIRT,WALL,
		WALL,DIRT,DIRT,DIRT,WALL,
		WALL,DIRT,DIRT,DIRT,WALL,
		WALL,WALL,WALL,WALL,WALL
	};
public:

	TileMap(std::string inputFile);

	TileMap();

	void UpdateTile(int row, int column, TileType tileType);

	void ResetTileMap();

	int* GetLevel();

};

