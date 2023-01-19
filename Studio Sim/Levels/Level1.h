#pragma once
#ifndef LEVEL1_H
#define LEVEL1_H

#include "Entity.h"
#include "EntityEditor.h"
#include "EntityController.h"

#include "Enemy.h"
#include "Camera.h"
#include "Player.h"
#include "UIEditor.h"
#include "TileMapDraw.h"
#include "TextRenderer.h"
#include "TileMapEditor.h"
#if _DEBUG
#include "AudioEditor.h"
#endif
#include "LevelStateMachine.h"
#include "CollisionHandler.h"

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
	void CleanUp() override;
  
private:
	// Tile Map
	void OnCreateTileMap(std::vector<TileMapDraw>& tileMapDraw);
	void UpdateTileMap(const float dt, std::vector<TileMapDraw>& tileMapDraw, TileMapLayer tileMapLayer);
	void RenderFrameTileMap(std::vector<TileMapDraw>& tileMapDraw);

	void OnCreateEntity();
	void RenderFrameEntity();
	void UpdateEntity(const float dt);

	// Objects
	std::vector<Entity> m_entity;
	Enemy m_enemy;
	Camera m_camera;
	Player m_player;
	
	UIEditor m_uiEditor;
	std::shared_ptr<UIScreen> m_uiScreen;

	CollisionHandler m_collisionHandler;
	ConstantBuffer<Matrices> m_cbMatrices;
#if _DEBUG
	AudioEditor m_audioEditor;
#endif
	int m_iEntityAmount;
	int m_iTileMapRows;
	int m_iTileMapColumns;
	EntityEditor m_entityEditor;
	EntityController m_entityController;

	TextRenderer m_textRenderer;
	TileMapEditor* m_tileMapEditor;
	std::vector<TileMapDraw> m_tileMapDrawBackground;
	std::vector<TileMapDraw> m_tileMapDrawForeground;
};

#endif