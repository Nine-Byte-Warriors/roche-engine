#include "stdafx.h"
#include "EntityEditor.h"

EntityEditor::EntityEditor()
{
	JsonLoading::LoadJson(m_vEntityData, JsonFile);

	m_vEntityDataCopy = m_vEntityData;
}

#if _DEBUG
void EntityEditor::SpawnControlWindow(float width, float height)
{
	m_fWidth = width;
	m_fHeight = height;

	if (ImGui::Begin("Entity Editor", FALSE, ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_HorizontalScrollbar))
	{
		EntityListBox();
		EntityWidget();

		ImGui::NewLine();
		AddNewEntity();
		SaveButton();		
	}

	ImGui::End();
}
#endif

void EntityEditor::EntityWidget()
{
#if _DEBUG
	for (int i = 0; i < m_vEntityData.size(); i++)
	{
		ImGui::PushID(i);
		if (ImGui::TreeNode(m_vEntityData[i].name.c_str()))
		{
			//m_iIdentifier = i;

			GetName();
			GetType();
			GetTexture();
			GetPosition();
			GetScale();
			GetMaxFrame();

			ImGui::TreePop();
		}
		ImGui::PopID();
	}
#endif
}

void EntityEditor::EntityListBox()
{
	if (ImGui::BeginListBox("##UI Screen List", ImVec2(-FLT_MIN, m_vEntityData.size() * ImGui::GetTextLineHeightWithSpacing() * 1.1f)))
	{
		int index = 0;
		for (int i = 0; i < m_vEntityData.size(); i++)
		{
			const bool isSelected = (i == index);
			if (ImGui::Selectable(m_vEntityData[i].name.c_str(), isSelected))
			{
				i = index;
				m_iIdentifier = i;
			}

			if (isSelected)
				ImGui::SetItemDefaultFocus();

			index++;
		}

		ImGui::EndListBox();
	}
}

void EntityEditor::AddNewEntity()
{
#if _DEBUG
	if (ImGui::Button("Add New Entity"))
	{
		EntityData* entityData;
		entityData = new EntityData();

		entityData->name = "Name";
		entityData->position.push_back(0.0f);
		entityData->position.push_back(0.0f);
		entityData->scale.push_back(0.0f);
		entityData->scale.push_back(0.0f);
		entityData->texture = "";
		entityData->type = "Type";
		entityData->identifier = m_vEntityData.size();
		entityData->maxFrame.push_back(0);
		entityData->maxFrame.push_back(0);

		m_vEntityData.push_back(*entityData);
		m_vEntityDataCopy.push_back(*entityData);

		delete entityData;
	}
#endif
}

void EntityEditor::GetName()
{
#if _DEBUG
	std::string name = m_vEntityDataCopy[m_iIdentifier].name;
	std::string displayText = "Name";
	std::string lable = "##Entity" + displayText + std::to_string(m_iIdentifier);
	std::string hint = name;
	ImGui::Text(displayText.c_str());

	static char entityName[128] = "";
	strcpy_s(entityName, name.c_str());

	ImGui::PushItemWidth(200.0f);
	ImGui::InputTextWithHint(lable.c_str(), hint.c_str(), entityName, IM_ARRAYSIZE(entityName));

	m_vEntityDataCopy[m_iIdentifier].name = entityName;
#endif
}

void EntityEditor::GetType()
{
#if _DEBUG
	ImGui::NewLine();

	std::string displayText = "Type";
	ImGui::Text(displayText.c_str());

	int entityType = 0;
	std::string previewEntityType = m_vEntityDataCopy[m_iIdentifier].type;
	const char* entityTypes[]{ "Player", "Enemy", "Projectile" };
	std::string lable = "##Entity" + displayText + std::to_string(m_iIdentifier);

	if (ImGui::BeginCombo(lable.c_str(), previewEntityType.c_str()))
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
			m_vEntityDataCopy[m_iIdentifier].type = "Player";
		}
		else if (entityType == 1)
		{
			m_vEntityDataCopy[m_iIdentifier].type = "Enemy";
		}
		else
		{
			m_vEntityDataCopy[m_iIdentifier].type = "Projectile";
		}
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
			m_sSelectedFile = m_sFilePath.substr(m_sFilePath.find("Resources\\Textures\\"));
			m_vEntityDataCopy[m_iIdentifier].texture = m_sSelectedFile;
			m_bValidTex = true;
		}
		else
		{
			m_sSelectedFile = "Open File Failed";
			m_bValidTex = false;
		}
	}
	ImGui::Text(m_sSelectedFile.c_str());
#endif
}

void EntityEditor::GetPosition()
{
#if _DEBUG
	ImGui::NewLine();
	ImGui::PushItemWidth(100.0f);

	std::string displayText = "Position";
	ImGui::Text(displayText.c_str());

	std::string lable = "##Entity" + displayText + "x" + std::to_string(m_iIdentifier);	
	ImGui::DragFloat(lable.c_str(), &m_vEntityDataCopy[m_iIdentifier].position[0], 1.0f, 0.0f, m_fWidth, "%.1f");

	ImGui::SameLine();

	lable = "##Entity" + displayText + "y" + std::to_string(m_iIdentifier);
	ImGui::DragFloat(lable.c_str(), &m_vEntityDataCopy[m_iIdentifier].position[1], 1.0f, 0.0f, m_fHeight, "%.1f");
#endif
}

void EntityEditor::GetScale()
{
#if _DEBUG
	ImGui::NewLine();
	ImGui::PushItemWidth(100.0f);

	std::string displayText = "Scale";
	ImGui::Text(displayText.c_str());

	std::string lable = "##Entity" + displayText + "x" + std::to_string(m_iIdentifier);
	ImGui::DragFloat(lable.c_str(), &m_vEntityDataCopy[m_iIdentifier].scale[0], 0.2f, 0.0f, m_fWidth, "%.1f");

	ImGui::SameLine();

	lable = "##Entity" + displayText + "y" + std::to_string(m_iIdentifier);
	ImGui::DragFloat(lable.c_str(), &m_vEntityDataCopy[m_iIdentifier].scale[1], 0.2f, 0.0f, m_fHeight, "%.1f");
#endif
}

void EntityEditor::GetMaxFrame()
{
#if _DEBUG
	ImGui::NewLine();

	std::string frameX = std::to_string(m_vEntityDataCopy[m_iIdentifier].maxFrame[0]);
	std::string frameY = std::to_string(m_vEntityDataCopy[m_iIdentifier].maxFrame[1]);

	std::string displayText = "Max Frame";
	std::string lable = "##Entity" + displayText + "x" + std::to_string(m_iIdentifier);
	std::string hint = frameX;
	ImGui::Text(displayText.c_str());

	static char entityMaxFrameX[128] = "";
	strcpy_s(entityMaxFrameX, frameX.c_str());

	ImGui::PushItemWidth(100.0f);
	ImGui::InputTextWithHint(lable.c_str(), hint.c_str(), entityMaxFrameX, IM_ARRAYSIZE(entityMaxFrameX));

	ImGui::SameLine();

	lable = "##Entity" + displayText + "y" + std::to_string(m_iIdentifier);
	hint = frameY;

	static char entityMaxFrameY[128] = "";
	strcpy_s(entityMaxFrameY, frameY.c_str());

	ImGui::InputTextWithHint(lable.c_str(), hint.c_str(), entityMaxFrameY, IM_ARRAYSIZE(entityMaxFrameY));

	try
	{
		std::string x(entityMaxFrameX);
		m_vEntityDataCopy[m_iIdentifier].maxFrame[0] = std::stof(x);
		std::string y(entityMaxFrameY);
		m_vEntityDataCopy[m_iIdentifier].maxFrame[1] = std::stof(y);
		m_bValidFrame = true;
	}
	catch (const std::exception&)
	{
		ImGui::Text("Not an int");
		m_bValidFrame = false;
	}
#endif
}

void EntityEditor::SaveButton()
{
#if _DEBUG

	bool SaveEntityButton = ImGui::Button("Save Entities");

	if (SaveEntityButton)
	{
		if (m_bValidTex && m_bValidFrame)
		{
			SaveEntity();			
		}
		else
		{
			ImGui::Button("Save Entity");
			m_sSavedText = "Invalid Data";
		}
	}

	ImGui::Text(m_sSavedText.c_str());
#endif
}

void EntityEditor::SaveEntity()
{
#if _DEBUG
	m_vEntityData = m_vEntityDataCopy;

	JsonLoading::SaveJson(m_vEntityData, JsonFile);
	m_sSavedText = "Saved";
#endif
}
