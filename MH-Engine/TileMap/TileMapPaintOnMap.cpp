#include "stdafx.h"
#include "TileMapPaintOnMap.h"

TileMapPaintOnMap::TileMapPaintOnMap()
{
	m_iBoarderTilesRows = 10;
	m_iBoarderTilesCols = 20;
	  
	m_iStartingPosX = -100;
	m_iStartingPosY = -100;

	m_bLeftMouseDown = false;	

	AddToEvent();
}

TileMapPaintOnMap::~TileMapPaintOnMap()
{
	RemoveFromEvent();
}

void TileMapPaintOnMap::Initialize(Camera& camera, int rows, int cols)
{
	m_camera = &camera;
	m_iRows = rows;
	m_iCols = cols;

	m_iSize = m_iRows * m_iCols;
}

int TileMapPaintOnMap::GetBoarderTilesRows()
{
	return m_iBoarderTilesRows;
}

int TileMapPaintOnMap::GetBoarderTilesCols()
{
	return m_iBoarderTilesCols;
}

int TileMapPaintOnMap::GetStartingPosX()
{
	return m_iStartingPosX;
}

int TileMapPaintOnMap::GetStartingPosY()
{
	return m_iStartingPosY;
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

void TileMapPaintOnMap::AddToEvent() noexcept
{
	EventSystem::Instance()->AddClient(EVENTID::ImGuiMousePosition, this);
	EventSystem::Instance()->AddClient(EVENTID::LeftMouseClick, this);
	EventSystem::Instance()->AddClient(EVENTID::LeftMouseRelease, this);
}

void TileMapPaintOnMap::RemoveFromEvent() noexcept
{
	EventSystem::Instance()->RemoveClient(EVENTID::ImGuiMousePosition, this);
	EventSystem::Instance()->RemoveClient(EVENTID::LeftMouseClick, this);
	EventSystem::Instance()->RemoveClient(EVENTID::LeftMouseRelease, this);
}

void TileMapPaintOnMap::HandleEvent(Event* event)
{
	switch (event->GetEventID())
	{
	case EVENTID::ImGuiMousePosition:
	{
		m_fCameraX = m_camera->GetPosition().x - m_camera->GetInitPosition().x - m_iStartingPosX;
		m_fCameraY = m_camera->GetPosition().y - m_camera->GetInitPosition().y - m_iStartingPosY;

		Vector2f mousePos = *static_cast<Vector2f*>(event->GetData());
		m_iTileX = (mousePos.x + m_fCameraX) / 32;
		m_iTileY = (mousePos.y + m_fCameraY) / 32;
		m_iPos = m_iTileX + m_iTileY * (m_iBoarderTilesCols + 40);
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
