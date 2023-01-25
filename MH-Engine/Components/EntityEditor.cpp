#include "stdafx.h"
#include "EntityEditor.h"
#include <FileHandler.h>

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
		AddNewEntity();
		ImGui::SameLine();
		SaveButton();

		EntityListBox();
		EntityWidget();
	}

	ImGui::End();
}
#endif

std::vector<EntityData> EntityEditor::GetEntityData()
{
#if _DEBUG
	return m_vEntityDataCopy;
#endif
}

bool EntityEditor::IsPositionLocked()
{
	return m_bLockPosition;
}

void EntityEditor::EntityWidget()
{
#if _DEBUG
	SpriteWidget();
	PhysicsWidget();
	AIWidget();
	ProjectileSystemWidget();
	ColliderWidget();
#endif
}

void EntityEditor::EntityListBox()
{
#if _DEBUG
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
#endif
}

void EntityEditor::AddNewEntity()
{
#if _DEBUG
	if (ImGui::Button("Add New Entity"))
	{
		EntityData* entityData;
		entityData = new EntityData();

		entityData->name = "Name" + std::to_string(m_vEntityData.size());
		entityData->position.push_back(0.0f);
		entityData->position.push_back(0.0f);
		entityData->scale.push_back(0.0f);
		entityData->scale.push_back(0.0f);
		entityData->texture = "";
		entityData->type = "Type";
		entityData->identifier = m_vEntityData.size();
		entityData->maxFrame.push_back(0);
		entityData->maxFrame.push_back(0);
		entityData->mass = 1.0f;
		entityData->behaviour = "None";

		m_vEntityData.push_back(*entityData);
		m_vEntityDataCopy.push_back(*entityData);

		delete entityData;
	}
#endif
}

void EntityEditor::SpriteWidget()
{
#if _DEBUG
	ImGui::NewLine();
	if (ImGui::TreeNode("Sprite"))
	{
		ImGui::NewLine();
		SetName();
		ImGui::NewLine();
		SetType();
		ImGui::NewLine();
		SetTexture();
		ImGui::NewLine();
		SetPosition();
		ImGui::NewLine();
		SetScale();
		ImGui::NewLine();
		SetMaxFrame();

		ImGui::TreePop();
	}
#endif
}

void EntityEditor::PhysicsWidget()
{
#if _DEBUG
	ImGui::NewLine();
	if (ImGui::TreeNode("Physics"))
	{
		ImGui::NewLine();
		SetMass();

		ImGui::TreePop();
	}
#endif
}

void EntityEditor::AIWidget()
{
#if _DEBUG
	if (m_vEntityDataCopy[m_iIdentifier].type == "Enemy")
	{
		ImGui::NewLine();
		if (ImGui::TreeNode("AI"))
		{
			ImGui::NewLine();
			SetBehaviour();

			ImGui::TreePop();
		}
	}
#endif
}

void EntityEditor::ProjectileSystemWidget()
{
#if _DEBUG
	ImGui::NewLine();
	if (ImGui::TreeNode("ProjectileSystem"))
	{


		ImGui::TreePop();
	}
#endif
}

void EntityEditor::ColliderWidget()
{
#if _DEBUG
	ImGui::NewLine();
	if (ImGui::TreeNode("Collider"))
	{


		ImGui::TreePop();
	}
#endif
}

void EntityEditor::SetName()
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

void EntityEditor::SetType()
{
#if _DEBUG
	std::string displayText = "Type";
	ImGui::Text(displayText.c_str());

	static int entityType = 0;
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
		else if (entityType == 2)
		{
			m_vEntityDataCopy[m_iIdentifier].type = "Projectile";
		}
	}
#endif
}

void EntityEditor::SetTexture()
{
#if _DEBUG
	ImGui::Text("Texture");
	m_sSelectedFile = m_vEntityDataCopy[m_iIdentifier].texture;
	if (ImGui::Button("Load Texture"))
	{
		std::shared_ptr<FileHandler::FileObject> foLoad = FileHandler::CreateFileObject(m_sSelectedFile);
		
		foLoad = FileHandler::FileDialog(foLoad)
			->UseOpenDialog()
			->ShowDialog()
			->StoreDialogResult();
		
		if (foLoad->HasPath())
		{
			std::string sFullPath = foLoad->GetFullPath();
			m_sSelectedFile = sFullPath.substr(sFullPath.find("Resources\\Textures\\"));
			m_vEntityDataCopy[m_iIdentifier].texture = m_sSelectedFile;
			m_bValidTex = true;
		}
		
		//if (FileLoading::OpenFileExplorer(m_sSelectedFile, m_sFilePath))
		//{
		//	m_sSelectedFile = m_sFilePath.substr(m_sFilePath.find("Resources\\Textures\\"));
		//	m_vEntityDataCopy[m_iIdentifier].texture = m_sSelectedFile;
		//	m_bValidTex = true;
		//}
		else
		{
			m_sSelectedFile = "Open File Failed";
			m_bValidTex = false;
		}
	}
	ImGui::Text(m_sSelectedFile.c_str());
#endif
}

void EntityEditor::SetPosition()
{
#if _DEBUG
	ImGui::PushItemWidth(100.0f);

	std::string displayText = "Position";
	ImGui::Text(displayText.c_str());

	std::string lable = "##Entity" + displayText + "x" + std::to_string(m_iIdentifier);	
	ImGui::DragFloat(lable.c_str(), &m_vEntityDataCopy[m_iIdentifier].position[0], 1.0f, -m_fWidth, m_fWidth, "%.1f");

	ImGui::SameLine();

	lable = "##Entity" + displayText + "y" + std::to_string(m_iIdentifier);
	ImGui::DragFloat(lable.c_str(), &m_vEntityDataCopy[m_iIdentifier].position[1], 1.0f, -m_fHeight, m_fHeight, "%.1f");

	ImGui::SameLine();

	ImGui::Checkbox("Lock", &m_bLockPosition);
#endif
}

void EntityEditor::SetScale()
{
#if _DEBUG
	int maxSize = 10000;

	ImGui::PushItemWidth(100.0f);

	std::string displayText = "Scale";
	ImGui::Text(displayText.c_str());

	std::string lable = "##Entity" + displayText + "x" + std::to_string(m_iIdentifier);
	ImGui::DragFloat(lable.c_str(), &m_vEntityDataCopy[m_iIdentifier].scale[0], 0.2f, 0.0f, maxSize, "%.1f");

	ImGui::SameLine();

	lable = "##Entity" + displayText + "y" + std::to_string(m_iIdentifier);
	ImGui::DragFloat(lable.c_str(), &m_vEntityDataCopy[m_iIdentifier].scale[1], 0.2f, 0.0f, maxSize, "%.1f");
#endif
}

void EntityEditor::SetMaxFrame()
{
#if _DEBUG
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

void EntityEditor::SetMass()
{
#if _DEBUG
	int maxMass = 1000;
	ImGui::PushItemWidth(200.0f);

	std::string displayText = "Mass";
	ImGui::Text(displayText.c_str());

	std::string lable = "##Entity" + displayText + std::to_string(m_iIdentifier);
	ImGui::DragFloat(lable.c_str(), &m_vEntityDataCopy[m_iIdentifier].mass, 0.2f, 1.0f, maxMass, "%.1f");
#endif
}

void EntityEditor::SetBehaviour()
{
#if _DEBUG
	std::string displayText = "Behaviour";
	ImGui::Text(displayText.c_str());

	static int activeBehaviour = 0;
	std::string previewEntityBehaviour = m_vEntityDataCopy[m_iIdentifier].behaviour;
	const char* behaviourList[]{ "Idle", "Seek", "Flee", "Patrol", "Follow", "Wander" };
	std::string lable = "##Entity" + displayText + std::to_string(m_iIdentifier);

	if (ImGui::BeginCombo(lable.c_str(), previewEntityBehaviour.c_str()))
	{
		for (int i = 0; i < IM_ARRAYSIZE(behaviourList); i++)
		{
			const bool isSelected = i == activeBehaviour;
			if (ImGui::Selectable(behaviourList[i], isSelected))
			{
				activeBehaviour = i;
				previewEntityBehaviour = behaviourList[i];
			}
		}

		ImGui::EndCombo();

		switch (activeBehaviour)
		{
		case 0:
			m_vEntityDataCopy[m_iIdentifier].behaviour = "Idle";
			break;
		case 1:
			m_vEntityDataCopy[m_iIdentifier].behaviour = "Seek";
			break;
		case 2:
			m_vEntityDataCopy[m_iIdentifier].behaviour = "Flee";
			break;
		case 3:
			m_vEntityDataCopy[m_iIdentifier].behaviour = "Patrol";
			break;
		case 4:
			m_vEntityDataCopy[m_iIdentifier].behaviour = "Follow";
			break;
		case 5:
			m_vEntityDataCopy[m_iIdentifier].behaviour = "Wander";
			break;
		}
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
