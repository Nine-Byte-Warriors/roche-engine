#pragma once
#ifndef ENTITYEDITOR_H
#define ENTITYEDITOR_H

#include "FileLoading.h"
#include "JsonLoading.h"
#include "Entity.h"
#include "AIStateMachine.h"
#include "AudioEngine.h"

class EntityEditor
{
public:
	EntityEditor();

#if _DEBUG
	void SpawnControlWindow(float width, float height);
#endif

	std::vector<EntityData> GetEntityData();
	void SetJsonFile( const std::string& name );
	bool IsPositionLocked();

	std::vector<int> GetEntitiesDeleted();
	void ClearEntitiesDeleted();

private:
	void EntityListBox();
	void EntityWidget();

	void PopulateProjectileList();

	void LockPositon();

	void SpriteWidget();
	void AnimationWidget();
	void PhysicsWidget();
	void AIWidget();
	void ProjectileSystemWidget();
	void ColliderWidget();
	void AudioWidget();

	//Sprite
	void SetName();
	void SetType();
	void SetPosition();
	void SetRotation();
	void SetTexture();
	void SetAnimation();
	void SetAnimationType();
	void SetScale();
	void SetRowsColumns();
	void SetHealth();

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
	void SetColliderTrigger();

	//Audio
	void SetSoundBank();

	void SaveButton();
	void SaveEntity();

	void AddNewEntity();
	void RemoveEntity();

	std::string JsonFile = "Entity.json";

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

	float m_fPushItemWidthFull = 210.0f;
	float m_fPushItemWidthHalf = 100.0f;

	int m_iRotation;

	int m_iSelectedIndex = 0;
	std::vector<std::string> m_projectileList;

	std::vector<std::string> m_vSoundBankNamesList;

	std::vector<int> m_entitiesDeleted;
};

#endif