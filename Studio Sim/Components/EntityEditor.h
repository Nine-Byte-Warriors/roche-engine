#pragma once
#ifndef ENTITYEDITOR_H
#define ENTITYEDITOR_H

#include "FileLoading.h"
#include "JsonLoading.h"
#include "Entity.h"
#if _DEBUG
#include <imgui/imgui.h>
#endif

class EntityEditor
{
public:
	EntityEditor();
	~EntityEditor();

#if _DEBUG
	void SpawnControlWindow();
#endif

private:
	void GetName();
	void GetType();
	void GetPosition();
	void GetTexture();
	void GetScale();
	void Save();
	void PopulateEntityDataWithNewEntity();

	std::string JsonFile = "Resources\\Entity\\Entity.json";

	std::vector<EntityData> m_vEntityData;

	EntityData m_entityData;

	std::string m_sSelectedFile = "LoadFile.txt";
	std::string m_sFilePath = "";

	bool m_bValidData;
	std::string m_sSavedText = "";

	std::string m_sName;
	std::string m_sType;
	float m_fPosX;
	float m_fPosY;
	float m_fScaleX;
	float m_fScaleY;
};

#endif