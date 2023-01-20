#include "stdafx.h"
#include "EntityEditor.h"

EntityEditor::EntityEditor()
{
	JsonLoading::LoadJson(m_vEntityData, JsonFile);
}

EntityEditor::~EntityEditor()
{
}

#if _DEBUG
void EntityEditor::SpawnControlWindow()
{
	if (ImGui::Begin("Entity Editor", FALSE, ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_HorizontalScrollbar))
	{
		GetName();
		GetType();
		GetTexture();
		GetPosition();
		GetScale();
		Save();
	}

	ImGui::End();
}
#endif

void EntityEditor::GetName()
{
#if _DEBUG
	std::string displayText = "Name";
	std::string lable = "##Entity" + displayText;
	std::string hint = "Enemy0";
	ImGui::Text(displayText.c_str());

	static char entityName[128] = "";
	ImGui::InputTextWithHint(lable.c_str(), hint.c_str(), entityName, IM_ARRAYSIZE(entityName));

	m_sName = entityName;
#endif
}

void EntityEditor::GetType()
{
#if _DEBUG
	ImGui::NewLine();

	ImGui::Text("Type");
	static int entityType = 0;
	static std::string previewEntityType = "Player";
	static const char* entityTypes[]{ "Player", "Enemy", "Projectile" };
	if (ImGui::BeginCombo("##SelectEntityType", previewEntityType.c_str()))
	{
		for (int i = 0; i < IM_ARRAYSIZE(entityTypes); i++)
		{
			const bool isSelected = i == entityType;
			if (ImGui::Selectable(entityTypes[i], isSelected))
			{
				entityType = i;
				previewEntityType = entityTypes[i];
			}
		}
		ImGui::EndCombo();

		if (entityType == 0)
		{
			m_sType = "Player";
		}
		else if (entityType == 1)
		{
			m_sType = "Enemy";
		}
		else
		{
			m_sType = "Projectile";
		}
	}
#endif
}

void EntityEditor::GetPosition()
{
#if _DEBUG
	ImGui::NewLine();

	std::string displayText = "Position";
	std::string lable = "##Entity" + displayText + "x";
	std::string hint = "x";
	ImGui::Text(displayText.c_str());

	static char entityPositionX[128] = "";
	ImGui::PushItemWidth(100.0f);
	ImGui::InputTextWithHint(lable.c_str(), hint.c_str(), entityPositionX, IM_ARRAYSIZE(entityPositionX));

	ImGui::SameLine();
	static char entityPositionY[128] = "";
	lable = "##Entity" + displayText + "y";
	hint = "y";
	ImGui::InputTextWithHint(lable.c_str(), hint.c_str(), entityPositionY, IM_ARRAYSIZE(entityPositionY));

	try
	{
		std::string x(entityPositionX);
		m_fPosX = std::stof(x);
		std::string y(entityPositionY);
		m_fPosY = std::stof(y);
		m_bValidData = true;
	}
	catch (const std::exception&)
	{
		ImGui::Text("Not a float");
		m_bValidData = false;
	}
#endif
}

void EntityEditor::GetTexture()
{
#if _DEBUG
	ImGui::NewLine();

	ImGui::Text("Texture");
	if (ImGui::Button("Load Texture"))
	{
		if (FileLoading::OpenFileExplorer(m_sSelectedFile, m_sFilePath))
		{
			m_sSelectedFile = "Resources\\Textures\\" + m_sSelectedFile;
		}
		else
		{
			m_sSelectedFile = "Open File Failed";
			m_bValidData = false;
		}			
	}
	ImGui::Text(m_sSelectedFile.c_str());
#endif
}

void EntityEditor::GetScale()
{
#if _DEBUG
	ImGui::NewLine();

	std::string displayText = "Scale";
	std::string lable = "##Entity" + displayText + "x";
	std::string hint = "x";
	ImGui::Text(displayText.c_str());

	static char entityPositionX[128] = "";
	ImGui::PushItemWidth(100.0f);
	ImGui::InputTextWithHint(lable.c_str(), hint.c_str(), entityPositionX, IM_ARRAYSIZE(entityPositionX));

	ImGui::SameLine();
	static char entityPositionY[128] = "";
	lable = "##Entity" + displayText + "y";
	hint = "y";
	ImGui::InputTextWithHint(lable.c_str(), hint.c_str(), entityPositionY, IM_ARRAYSIZE(entityPositionY));

	try
	{
		std::string x(entityPositionX);
		m_fScaleX = std::stof(x);
		std::string y(entityPositionY);
		m_fScaleY = std::stof(y);
		m_bValidData = true;
	}
	catch (const std::exception&)
	{
		ImGui::Text("Not a float");
		m_bValidData = false;
	}
#endif
}

void EntityEditor::Save()
{
#if _DEBUG
	ImGui::NewLine();
	ImGui::Text("Save");

	bool SaveEntityButton = ImGui::Button("Save Entity");

	if (SaveEntityButton && m_bValidData)
	{
		PopulateEntityDataWithNewEntity();

		JsonLoading::SaveJson(m_vEntityData, JsonFile);
		m_sSavedText = "Saved";
	}
	else if (SaveEntityButton)
	{
		ImGui::Button("Save Entity");
		m_sSavedText = "Invalid Data";
	}

	ImGui::Text(m_sSavedText.c_str());
#endif
}

void EntityEditor::PopulateEntityDataWithNewEntity()
{
	m_entityData.name = m_sName;
	m_entityData.position.push_back(m_fPosX);
	m_entityData.position.push_back(m_fPosY);
	m_entityData.scale.push_back(m_fScaleX);
	m_entityData.scale.push_back(m_fScaleY);
	m_entityData.texture = m_sSelectedFile;
	m_entityData.type = m_sType;

	m_vEntityData.push_back(m_entityData);
}
