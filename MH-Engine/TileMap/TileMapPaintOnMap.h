#pragma once
#ifndef TILEMAPPAINTONMAP_H
#define TILEMAPPAINTONMAP_H

#include "EventSystem.h"
#include "Camera.h"
#include <Vector2f.h>

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

	int m_iBoarderTilesRows;
	int m_iBoarderTilesCols;

	int m_iStartingPosX;
	int m_iStartingPosY;

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
};

#endif