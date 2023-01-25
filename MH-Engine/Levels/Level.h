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
#include "TileMapLoader.h"
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

	inline bool GetIsFirstLoad() const noexcept { return m_bFirstLoad; }
#if _DEBUG
	inline void SetAudioJson( const std::string& name ) noexcept { m_audioEditor.SetJsonFile( name ); }
#endif
	inline void SetEntityJson( const std::string& name ) noexcept { m_entityEditor.SetJsonFile( name ); m_entityController.SetJsonFile( name ); }
	//inline void SetTileMapJson( const std::string& name ) noexcept { m_tileMapEditor->SetJsonFile( name ); }
	inline void SetTileMapJson( const std::string& back, const std::string& front ) noexcept { m_tileMapLoader.LoadLevel( back, front ); }
	inline void SetUIJson( const std::string& name ) noexcept { m_uiEditor.SetJsonFile( name ); }

	void CreateEntity();
	void CreateTileMap();
	void CreateUI();

private:
	void RenderFrameEntity();
	void UpdateEntity(const float dt);
	void UpdateEntityFromEditor(const float dt);

	// Tile Map
	void CreateTileMapDraw();
	void UpdateTileMap(const float dt);
	void UpdateBothTileMaps(const float dt);
	void UpdateTileMapTexture(const float dt);
	void UpdateTileMapEmpty(const float dt);
	void RenderFrameTileMap();

	// Objects
	Enemy m_enemy;
	Camera m_camera;
	Player m_player;
	UIEditor m_uiEditor;
	std::vector<Entity> m_entity;

	CollisionHandler m_collisionHandler;
	ConstantBuffer<Matrices> m_cbMatrices;

#if _DEBUG
	int m_iUpdateBothTileMapLayers;
	AudioEditor m_audioEditor;
#endif
	bool m_bFirstLoad = true;
	int m_iFirstTimeTileMapDrawBothLayers;

	int m_iEntityAmount;
	int m_iTileMapRows;
	int m_iTileMapColumns;
	EntityEditor m_entityEditor;
	EntityController m_entityController;
	std::shared_ptr<ProjectileEditor> m_projectileEditor;

	TextRenderer m_textRenderer;
	TileMapEditor m_tileMapEditor;
	TileMapLoader m_tileMapLoader;
	std::vector<std::vector<TileMapDraw>> m_tileMapDrawLayers;
	const int m_iTileMapLayers = 2;
	const int m_iTileSize = 32;
	bool m_bMapUpdate = true;
};

#endif