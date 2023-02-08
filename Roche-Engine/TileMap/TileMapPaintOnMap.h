#pragma once
#ifndef TILEMAPPAINTONMAP_H
#define TILEMAPPAINTONMAP_H

#include "Camera.h"

class TileMapPaintOnMap : public Listener
{
public:
	TileMapPaintOnMap();
	~TileMapPaintOnMap();
	void Initialize(Camera& camera, int rows, int cols, int startingPosX, int startingPosY);

	int GetTileMapPos();

	bool IsLeftMouseDown();
	bool IsNearTheMouse(Vector2f pos, Vector2f offSet, float radius);
	Vector2f GetMapPos(Vector2f pos, Vector2f offSet);

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

	Vector2f m_fMousePos;

	Camera* m_camera;

	int m_iSize;
	int m_iRows;
	int m_iCols;
};

#endif