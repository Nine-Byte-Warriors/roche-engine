#pragma once
#ifndef ENTITYEDITOR_H
#define ENTITYEDITOR_H

#include "FileLoading.h"
#include "JsonLoading.h"
#include "Entity.h"
#include "AIStateMachine.h"
#if _DEBUG
#include <imgui/imgui.h>
#endif

class EntityEditor
{
public:
	EntityEditor();

#if _DEBUG
	void SpawnControlWindow(float width, float height);
#endif

	std::vector<EntityData> GetEntityData();

	bool IsPositionLocked();

private:
	void EntityListBox();
	void EntityWidget();

	void PopulateProjectileList();

	void LockPositon();

	void SpriteWidget();
	void PhysicsWidget();
	void AIWidget();
	void ProjectileSystemWidget();
	void ColliderWidget();

	//Sprite
	void SetName();
	void SetType();
	void SetPosition();
	void SetTexture();
	void SetScale();
	void SetMaxFrame();

	//Physics
	void SetMass();
	void SetSpeed();

	//AI
	void SetBehaviour();

	//ProjectileSystem
	void SetProjectilePattern();
	void SetProjectileBullet();

	//Collider
	void SetColliderShape();
	void SetColliderSize();

	void SaveButton();
	void SaveEntity();

	void AddNewEntity();
	void RemoveEntity();

	std::string JsonFile = "Resources\\Entity\\Entity.json";

	std::vector<EntityData> m_vEntityData;

	std::string m_sSelectedFileTex = "LoadFile.txt";
	std::string m_sFilePathTex = "";

	std::string m_sSelectedFileProjectile = "LoadFile.txt";
	std::string m_sFilePathProjectile = "";

	bool m_bValidFrame;
	bool m_bValidTex;
	bool m_bValidProjectile;

	std::string m_sSavedText = "";

	int m_iIdentifier = 0;

	std::vector<EntityData> m_vEntityDataCopy;

	float m_fWidth;
	float m_fHeight;

	bool m_bIsUpdated;
	bool m_bLockPosition;
	bool m_bLockToScale;

	std::vector<std::string> m_projectileList;
};

#endif