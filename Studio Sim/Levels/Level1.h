#pragma once
#ifndef LEVEL1_H
#define LEVEL1_H

#include "Enemy.h"
#include "Camera.h"
#include "Player.h"
#include "TileMapDraw.h"
#include "TextRenderer.h"
#include "LevelStateMachine.h"
#include "TileMapEditor.h"

/// <summary>
/// The first level of the game.
/// Inherits from Level to render/update objects used in each level.
/// </summary>
class Level1 : public LevelContainer
{
public:
	Level1() {}

	void OnCreate() override;
	void OnSwitch() override;
	
	void BeginFrame() override;
	void RenderFrame() override;
	void EndFrame() override;

	void Update( const float dt ) override;
	void CleanUp() override {}
private:
	// Tile Map
	void OnCreateTileMap();
	void UpdateTileMap(const float dt);
	void RenderFrameTileMap();

	// Objects
	Enemy m_enemy;
	Camera m_camera;
	Player m_player;
	TextRenderer m_textRenderer;
	TileMapEditor m_tileMapEditor;
	ConstantBuffer<Matrices> m_cbMatrices;
	std::vector<TileMapDraw> m_tileMapDraw;
};

#endif