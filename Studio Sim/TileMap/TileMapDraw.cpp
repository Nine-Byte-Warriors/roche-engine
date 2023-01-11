#include "stdafx.h"
#include "TileMapDraw.h"

TileMapDraw::TileMapDraw()
{
	m_sprite = std::make_shared<Sprite>();
	m_transform = std::make_shared<Transform>();

	m_transform->SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
	m_transform->SetRotation(XMFLOAT3(0.0f, 0.0f, 0.0f));
	m_transform->SetScale(XMFLOAT3(1.0f, 1.0f, 1.0f));
	m_transform->Update(); // Initial updated required
}

void TileMapDraw::Update(const float dt)
{
	m_transform->Update();
}