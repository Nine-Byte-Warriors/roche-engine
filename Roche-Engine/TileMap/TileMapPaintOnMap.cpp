#include "stdafx.h"
#include "TileMapPaintOnMap.h"

#if _DEBUG
extern bool g_bDebug;
#endif

#define FOLDER_PATH "Resources\\TileMaps\\"

TileMapPaintOnMap::TileMapPaintOnMap()
{
	m_bLeftMouseDown = false;

	AddToEvent();
}

TileMapPaintOnMap::~TileMapPaintOnMap()
{
	RemoveFromEvent();
}

void TileMapPaintOnMap::Initialize(Camera& camera, int rows, int cols, int startingPosY, int startingPosX)
{
	m_camera = &camera;
	m_iRows = rows;
	m_iCols = cols;

	m_iStartingPosX = startingPosX;
	m_iStartingPosY = startingPosY;

	m_iSize = m_iRows * m_iCols;
}

int TileMapPaintOnMap::GetTileMapPos()
{
	return m_iPos;
}

bool TileMapPaintOnMap::IsLeftMouseDown()
{
	bool isMouseInPlayerArea = m_iTileX >= 0 && m_iTileX < m_iCols&& m_iTileY >= 0 && m_iTileY < m_iRows;
	if (isMouseInPlayerArea)
	{
		return m_bLeftMouseDown;
	}
	return false;
}

bool TileMapPaintOnMap::IsNearTheMouse(Vector2f pos, Vector2f offSet, float radius)
{
	Vector2f mouseLocation = Vector2f(m_camera->GetPosition().x - m_camera->GetInitPosition().x + m_fMousePos.x,
		m_camera->GetPosition().y - m_camera->GetInitPosition().y + m_fMousePos.y);
	float distance = mouseLocation.Distance(pos + offSet);

	if (distance <= radius)
	{
		return true;
	}
	return false;
}

Vector2f TileMapPaintOnMap::GetMapPos(Vector2f pos, Vector2f offSet)
{
	Vector2f mouseLocation = Vector2f(m_camera->GetPosition().x - m_camera->GetInitPosition().x + m_fMousePos.x,
		m_camera->GetPosition().y - m_camera->GetInitPosition().y + m_fMousePos.y);
	mouseLocation -= offSet;

	return mouseLocation;
}

int TileMapPaintOnMap::GetPositionAtCoordinates(int x, int y)
{
	float cameraX = m_camera->GetPosition().x - m_camera->GetInitPosition().x + m_iStartingPosX;
	float cameraY = m_camera->GetPosition().y - m_camera->GetInitPosition().y + m_iStartingPosY;
	int tileX = (cameraX + x) / 32;
	int tileY = ceilf((cameraY + y) / 32);
	int pos = tileX + tileY * m_iCols;

	if (pos >= m_iCols * m_iRows)
	{
		pos = m_iCols * m_iRows;
	}
	if (pos < 0)
	{
		pos = 0;
	}

	return pos;
}

void TileMapPaintOnMap::AddToEvent() noexcept
{
	EventSystem::Instance()->AddClient(EVENTID::MouseCameraPosition, this);
	EventSystem::Instance()->AddClient(EVENTID::LeftMouseClick, this);
	EventSystem::Instance()->AddClient(EVENTID::LeftMouseRelease, this);
}

void TileMapPaintOnMap::RemoveFromEvent() noexcept
{
	EventSystem::Instance()->RemoveClient(EVENTID::MouseCameraPosition, this);
	EventSystem::Instance()->RemoveClient(EVENTID::LeftMouseClick, this);
	EventSystem::Instance()->RemoveClient(EVENTID::LeftMouseRelease, this);
}

void TileMapPaintOnMap::HandleEvent(Event* event)
{
	switch (event->GetEventID())
	{
	case EVENTID::MouseCameraPosition:
	{
		Vector2f mousePos2 = *static_cast<Vector2f*>(event->GetData());

		m_iTileX = (mousePos2.x + m_iStartingPosX) / 32;
		m_iTileY = (mousePos2.y + m_iStartingPosY) / 32;
		m_iPos = m_iTileX + m_iTileY * m_iCols;
	}
	break;
	case EVENTID::LeftMouseClick:
	{
		m_bLeftMouseDown = true;
	}
	break;
	case EVENTID::LeftMouseRelease:
	{
		m_bLeftMouseDown = false;
	}
	break;
	}
}
