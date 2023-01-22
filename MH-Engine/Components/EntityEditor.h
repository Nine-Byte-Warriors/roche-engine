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

	//AI
	void SetBehaviour();

	void SaveButton();
	void SaveEntity();

	void AddNewEntity();

	std::string JsonFile = "Resources\\Entity\\Entity.json";

	std::vector<EntityData> m_vEntityData;

	std::string m_sSelectedFile = "LoadFile.txt";
	std::string m_sFilePath = "";

	bool m_bValidFrame;
	bool m_bValidTex;

	std::string m_sSavedText = "";

	int m_iIdentifier = 0;

	std::vector<EntityData> m_vEntityDataCopy;

	float m_fWidth;
	float m_fHeight;

	bool m_bIsUpdated;
	bool m_bLockPosition;
};

#endif