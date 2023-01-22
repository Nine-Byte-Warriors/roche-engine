#include "stdafx.h"
#include "TileMapDraw.h"
#include "Graphics.h"

TileMapDraw::TileMapDraw()
{
	m_sprite = std::make_shared<Sprite>();
	m_transform = std::make_shared<Transform>( m_sprite );
}

void TileMapDraw::Initialize( const Graphics& gfx, ConstantBuffer<Matrices>& mat, const std::string& path )
{
	m_sprite->Initialize( gfx.GetDevice(), gfx.GetContext(), path, mat );
}

void TileMapDraw::Update(const float dt)
{
	m_transform->Update();
}