#pragma once
#ifndef LEVEL_H
#define LEVEL_H

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
#include "ProjectileEditor.h"
#include "LevelStateMachine.h"
#include "CollisionHandler.h"


/// <summary>
/// The first level of the game.
/// Inherits from Level to render/update objects used in each level.
/// </summary>
class Level : public LevelContainer
{
public:
	Level( const std::string& name, int& levelId )
	{
		m_sLevelName = name;
		levelId++;
		m_iCurrentLevel = levelId;
		m_iNextLevel = levelId + 1;
	}

	void OnCreate() override;
	void OnSwitch() override;
	
	void BeginFrame() override;
	void RenderFrame() override;
	void EndFrame_Start() override;
	void EndFrame_End() override;

	void Update( const float dt ) override;
	void CleanUp() override {}
  
private:
	// Tile Map
	void OnCreateTileMap(std::vector<TileMapDraw>& tileMapDraw);
	void UpdateTileMap(const float dt, std::vector<TileMapDraw>& tileMapDraw, TileMapLayer tileMapLayer);
	void RenderFrameTileMap(std::vector<TileMapDraw>& tileMapDraw);

	void OnCreateEntity();
	void RenderFrameEntity();
	void UpdateEntity(const float dt);
	void UpdateEntityFromEditor(const float dt);

	// Objects
	Enemy m_enemy;
	Camera m_camera;
	Player m_player;
	UIEditor m_uiEditor;
	std::vector<Entity> m_entity;

	CollisionHandler m_collisionHandler;
	ConstantBuffer<Matrices> m_cbMatrices;
#if _DEBUG
	AudioEditor m_audioEditor;
#endif
	int m_iEntityAmount;
	int m_iTileMapRows;
	int m_iTileMapColumns;
	bool m_bFirstLoad = true;
	EntityEditor m_entityEditor;
	EntityController m_entityController;

	int m_iFirstTimeTileMapDrawBothLayers;
#if _DEBUG
	int m_iUpdateBothTileMapLayers;
#endif

	TextRenderer m_textRenderer;
	TileMapEditor m_tileMapEditor;
	std::vector<TileMapDraw> m_tileMapDrawBackground;
	std::vector<TileMapDraw> m_tileMapDrawForeground;
	std::shared_ptr<ProjectileEditor> m_projectileEditor;
};

#endif