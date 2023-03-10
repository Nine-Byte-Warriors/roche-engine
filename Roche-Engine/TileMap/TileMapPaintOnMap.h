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
	void Initialize(Camera& camera, int rows, int cols, int startingPosX, int startingPosY);

	int GetTileMapPos();

	bool IsLeftMouseDown();

	int GetPositionAtCoordinates(int x, int y);

private:
	void AddToEvent() noexcept;
	void RemoveFromEvent() noexcept;
	void HandleEvent(Event* event) override;

	float m_fCameraX;
	float m_fCameraY;
	int m_iTileX;
	int m_iTileY;
	int m_iPos;
	
	int m_iStartingPosX;
	int m_iStartingPosY;

	bool m_bLeftMouseDown;

	Camera* m_camera;

	int m_iSize;
	int m_iRows;
	int m_iCols;
};

#endif