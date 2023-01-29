#pragma once
#ifndef TILEMAPPAINTONMAP_H
#define TILEMAPPAINTONMAP_H

#include "EventSystem.h"
#include "Camera.h"
#include "JsonLoading.h"
#include <Vector2f.h>

struct TileMapPaint
{
	int boarderTilesRows;
	int boarderTilesCols;

	int startingPosX;
	int startingPosY;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TileMapPaint, boarderTilesRows, boarderTilesCols, startingPosX, startingPosY)

class TileMapPaintOnMap : public Listener
{
public:
	TileMapPaintOnMap();
	~TileMapPaintOnMap();
	void Initialize(Camera& camera, int rows, int cols);

	int GetBoarderTilesRows();
	int GetBoarderTilesCols();

	int GetStartingPosX();
	int GetStartingPosY();

	int GetTileMapPos();

	bool IsLeftMouseDown();

private:
	void AddToEvent() noexcept;
	void RemoveFromEvent() noexcept;
	void HandleEvent(Event* event) override;

	float m_fCameraX;
	float m_fCameraY;
	int m_iTileX;
	int m_iTileY;
	int m_iPos;

	bool m_bLeftMouseDown;

	Camera* m_camera;

	int m_iSize;
	int m_iRows;
	int m_iCols;

	std::string JsonFile = "TileMapPaint.json";
	TileMapPaint m_tileMapPaint;
};

#endif