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

#if _DEBUG
	void SpawnControlWindow(float width, float height);
#endif

private:
	void EntityWidget();
	void EntityListBox();
	void AddNewEntity();

	void GetName();
	void GetType();
	void GetPosition();
	void GetTexture();
	void GetScale();
	void GetMaxFrame();

	void SaveButton();
	void SaveEntity();

	std::string JsonFile = "Resources\\Entity\\Entity.json";

	std::vector<EntityData> m_vEntityData;

	std::string m_sSelectedFile = "LoadFile.txt";
	std::string m_sFilePath = "";

	bool m_bValidFrame;
	bool m_bValidTex;

	std::string m_sSavedText = "";

	int m_iIdentifier;

	std::vector<EntityData> m_vEntityDataCopy;

	float m_fWidth;
	float m_fHeight;
};

#endif