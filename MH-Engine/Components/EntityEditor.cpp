#include "stdafx.h"
#include "EntityEditor.h"
#include <FileHandler.h>

#define FOLDER_PATH "Resources\\Entity\\"

EntityEditor::EntityEditor()
{
	m_vEntityData.clear();
	JsonLoading::LoadJson(m_vEntityData, FOLDER_PATH + JsonFile);
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
		RemoveEntity();
		SaveButton();

		LockPositon();

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

void EntityEditor::SetJsonFile( const std::string& name )
{
	JsonFile = name;
	m_vEntityData.clear();
	JsonLoading::LoadJson(m_vEntityData, FOLDER_PATH + JsonFile);
	m_vEntityDataCopy = m_vEntityData;
}

bool EntityEditor::IsPositionLocked()
{
	return m_bLockPosition;
}

void EntityEditor::EntityWidget()
{
#if _DEBUG
	SetType();
	SpriteWidget();
	PhysicsWidget();
	AIWidget();
	ProjectileSystemWidget();
	ColliderWidget();
#endif
}

void EntityEditor::PopulateProjectileList()
{
	m_projectileList.clear();

	for (int i = 0; i < m_vEntityDataCopy.size(); i++)
	{
		if (m_vEntityDataCopy[i].type == "Projectile")
		{
			m_projectileList.push_back(m_vEntityDataCopy[i].name);
		}
	}
}

void EntityEditor::LockPositon()
{
#if _DEBUG
	ImGui::Checkbox("Reset/Lock Position", &m_bLockPosition);
#endif
}

void EntityEditor::EntityListBox()
{
#if _DEBUG
	if (ImGui::BeginListBox("##Entity List", ImVec2(-FLT_MIN, m_vEntityData.size() * ImGui::GetTextLineHeightWithSpacing() * 1.1f)))
	{
		for (int i = 0; i < m_vEntityData.size(); i++)
		{
			const bool isSelected = (i == m_iSelectedIndex);
			if (ImGui::Selectable(m_vEntityData[i].name.c_str(), isSelected))
			{
				m_iSelectedIndex = i;
				m_iIdentifier = i;
			}

			if (isSelected)
				ImGui::SetItemDefaultFocus();
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
		static int nameNum = m_vEntityData.size();

		entityData->name = "Name" + std::to_string(nameNum);
		entityData->position.push_back(500.0f);
		entityData->position.push_back(500.0f);
		entityData->scale.push_back(64.0f);
		entityData->scale.push_back(64.0f);
		entityData->texture = "None";
		entityData->type = "Enemy";
		entityData->identifier = m_vEntityData.size();
		entityData->maxFrame.push_back(1);
		entityData->maxFrame.push_back(1);
		entityData->mass = 1.0f;
		entityData->speed = 10.0f;
		entityData->behaviour = "None";
		entityData->colliderShape = "Circle";
		entityData->colliderRadius.push_back(64.0f);
		entityData->colliderRadius.push_back(64.0f);
		entityData->projectilePattern = "None";
		entityData->projectileBullet = "None";
		entityData->AI = true;
		entityData->projectileSystem = true;
		entityData->collider = false;
		entityData->bProjectilePattern = true;
		entityData->bProjectileBullet = true;

		m_vEntityData.push_back(*entityData);
		m_vEntityDataCopy.push_back(*entityData);
		nameNum++;

		delete entityData;
	}
#endif
}

void EntityEditor::RemoveEntity()
{
#if _DEBUG
	if (ImGui::Button("Remove Entity"))
	{
		m_entitiesDeleted.push_back(m_iIdentifier);
		m_vEntityDataCopy.erase(m_vEntityDataCopy.begin() + m_iIdentifier);
		m_vEntityData.erase(m_vEntityData.begin() + m_iIdentifier);

		for (int i = 0; i < m_vEntityData.size(); i++)
		{
			m_vEntityData[i].identifier = i;
			m_vEntityDataCopy[i].identifier = i;
		}

		m_iIdentifier = 0;
	}
#endif
}

void EntityEditor::ClearEntitiesDeleted()
{
#if _DEBUG
	m_entitiesDeleted.clear();
#endif
}

std::vector<int> EntityEditor::GetEntitiesDeleted()
{
#if _DEBUG
	return m_entitiesDeleted;
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
		ImGui::NewLine();
		SetSpeed();

		ImGui::TreePop();
	}
#endif
}

void EntityEditor::AIWidget()
{
#if _DEBUG
	ImGui::NewLine();
	ImGui::Checkbox("AI", &m_vEntityDataCopy[m_iIdentifier].AI);
	if (m_vEntityDataCopy[m_iIdentifier].AI)
	{
		ImGui::SameLine();
		if (ImGui::TreeNode("##AI"))
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
	ImGui::Checkbox("ProjectileSystem", &m_vEntityDataCopy[m_iIdentifier].projectileSystem);
	if (m_vEntityDataCopy[m_iIdentifier].projectileSystem)
	{
		ImGui::SameLine();
		if (ImGui::TreeNode("##ProjectileSystem"))
		{
			SetProjectilePattern();	
			SetProjectileBullet();
			
			ImGui::TreePop();
		}
	}
#endif
}

void EntityEditor::ColliderWidget()
{
#if _DEBUG
	ImGui::NewLine();
	ImGui::Checkbox("Collider", &m_vEntityDataCopy[m_iIdentifier].collider);
	if (m_vEntityDataCopy[m_iIdentifier].collider)
	{
		ImGui::SameLine();
		if (ImGui::TreeNode("##Collider"))
		{
			ImGui::NewLine();
			SetColliderShape();
			ImGui::NewLine();
			SetColliderSize();

			ImGui::TreePop();
		}
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
	ImGui::NewLine();

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

		m_vEntityDataCopy[m_iIdentifier].type = entityTypes[entityType];
	}
#endif
}

void EntityEditor::SetTexture()
{
#if _DEBUG
	ImGui::Text("Texture");
	m_sSelectedFileTex = m_vEntityDataCopy[m_iIdentifier].texture;
	if (ImGui::Button("Load Texture"))
	{
		std::shared_ptr<FileHandler::FileObject> foLoad = FileHandler::CreateFileObject(m_sSelectedFileTex);
		
		foLoad = FileHandler::FileDialog(foLoad)
			->UseOpenDialog()
			->ShowDialog()
			->StoreDialogResult();
		
		if (foLoad->HasPath())
		{
			std::string sFullPath = foLoad->GetFullPath();
			m_sSelectedFileTex = sFullPath.substr(sFullPath.find("Resources\\Textures\\"));
			m_vEntityDataCopy[m_iIdentifier].texture = m_sSelectedFileTex;
			m_bValidTex = true;
		}
		else
		{
			m_sSelectedFileTex = "Open File Failed";
			m_bValidTex = false;
		}
	}
	ImGui::Text(m_sSelectedFileTex.c_str());
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

void EntityEditor::SetSpeed()
{
#if _DEBUG
	int maxSpeed = 100;
	ImGui::PushItemWidth(200.0f);

	std::string displayText = "Speed";
	ImGui::Text(displayText.c_str());

	std::string lable = "##Entity" + displayText + std::to_string(m_iIdentifier);
	ImGui::DragFloat(lable.c_str(), &m_vEntityDataCopy[m_iIdentifier].speed, 0.2f, 1.0f, maxSpeed, "%.1f");
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

		m_vEntityDataCopy[m_iIdentifier].behaviour = behaviourList[activeBehaviour];
	}
#endif
}

void EntityEditor::SetProjectilePattern()
{
#if _DEBUG
	ImGui::NewLine();
	ImGui::Checkbox("Pattern", &m_vEntityDataCopy[m_iIdentifier].bProjectilePattern);

	if (m_vEntityDataCopy[m_iIdentifier].bProjectilePattern)
	{
		m_sSelectedFileProjectile = m_vEntityDataCopy[m_iIdentifier].projectilePattern;
		if (ImGui::Button("Load Pattern"))
		{
			if (FileLoading::OpenFileExplorer(m_sSelectedFileProjectile, m_sFilePathProjectile))
			{
				m_sSelectedFileProjectile = m_sFilePathProjectile.substr(m_sFilePathProjectile.find("Resources\\Patterns\\"));
				m_vEntityDataCopy[m_iIdentifier].projectilePattern = m_sSelectedFileProjectile;
				m_bValidProjectile = true;
			}
			else
			{
				m_sSelectedFileProjectile = "Open File Failed";
				m_bValidProjectile = false;
			}
		}
		ImGui::Text(m_sSelectedFileProjectile.c_str());
	}
#endif
}

void EntityEditor::SetProjectileBullet()
{
#if _DEBUG
	ImGui::NewLine();
	ImGui::Checkbox("Bullet", &m_vEntityDataCopy[m_iIdentifier].bProjectileBullet);

	if (m_vEntityDataCopy[m_iIdentifier].bProjectileBullet)
	{
		PopulateProjectileList();

		std::string displayText = "Bullet";

		static int activeProjectorBullet = 0;
		std::string previewEntityProjectileButtlet = m_vEntityDataCopy[m_iIdentifier].projectileBullet;

		std::string lable = "##Entity" + displayText + std::to_string(m_iIdentifier);

		if (ImGui::BeginCombo(lable.c_str(), previewEntityProjectileButtlet.c_str()))
		{
			for (int i = 0; i < m_projectileList.size(); i++)
			{
				const bool isSelected = i == activeProjectorBullet;
				if (ImGui::Selectable(m_projectileList[i].c_str(), isSelected))
				{
					activeProjectorBullet = i;
					previewEntityProjectileButtlet = m_projectileList[i];
				}
			}

			ImGui::EndCombo();

			m_vEntityDataCopy[m_iIdentifier].projectileBullet = m_projectileList[activeProjectorBullet];
		}
	}
#endif
}

void EntityEditor::SetColliderShape()
{
#if _DEBUG
	std::string displayText = "Collider Shape";
	ImGui::Text(displayText.c_str());

	static int activeCollider = 0;
	std::string previewEntityCollider = m_vEntityDataCopy[m_iIdentifier].colliderShape;
	const char* colliderList[]{ "Circle", "Box" };
	std::string lable = "##Entity" + displayText + std::to_string(m_iIdentifier);

	if (ImGui::BeginCombo(lable.c_str(), previewEntityCollider.c_str()))
	{
		for (int i = 0; i < IM_ARRAYSIZE(colliderList); i++)
		{
			const bool isSelected = i == activeCollider;
			if (ImGui::Selectable(colliderList[i], isSelected))
			{
				activeCollider = i;
				previewEntityCollider = colliderList[i];
			}
		}

		ImGui::EndCombo();

		m_vEntityDataCopy[m_iIdentifier].colliderShape = colliderList[activeCollider];
	}
#endif
}

void EntityEditor::SetColliderSize()
{
#if _DEBUG
	ImGui::Checkbox("Lock To Scale", &m_bLockToScale);

	if (m_bLockToScale)
	{
		m_vEntityDataCopy[m_iIdentifier].colliderRadius[0] = m_vEntityDataCopy[m_iIdentifier].scale[0];
		m_vEntityDataCopy[m_iIdentifier].colliderRadius[1] = m_vEntityDataCopy[m_iIdentifier].scale[1];
	}

	if (m_vEntityDataCopy[m_iIdentifier].colliderShape == "Circle")
	{
		ImGui::PushItemWidth(200.0f);

		std::string displayText = "Radius";
		ImGui::Text(displayText.c_str());

		std::string lable = "##Entity" + displayText + std::to_string(m_iIdentifier);
		ImGui::DragFloat(lable.c_str(), &m_vEntityDataCopy[m_iIdentifier].colliderRadius[0], 1.0f, -m_fWidth, m_fWidth, "%.1f");
	}
	else if (m_vEntityDataCopy[m_iIdentifier].colliderShape == "Box")
	{
		ImGui::PushItemWidth(100.0f);

		std::string displayText = "Radius";
		ImGui::Text(displayText.c_str());

		std::string lable = "##Entity" + displayText + "x" + std::to_string(m_iIdentifier);
		ImGui::DragFloat(lable.c_str(), &m_vEntityDataCopy[m_iIdentifier].colliderRadius[0], 1.0f, -m_fWidth, m_fWidth, "%.1f");

		ImGui::SameLine();

		lable = "##Entity" + displayText + "y" + std::to_string(m_iIdentifier);
		ImGui::DragFloat(lable.c_str(), &m_vEntityDataCopy[m_iIdentifier].colliderRadius[1], 1.0f, -m_fHeight, m_fHeight, "%.1f");
	}
#endif
}

void EntityEditor::SaveButton()
{
#if _DEBUG
	bool SaveEntityButton = ImGui::Button("Save Entities");

	if (SaveEntityButton)
	{
		if (m_bValidTex && m_bValidFrame && m_bValidProjectile)
		{
			SaveEntity();			
		}
		else
		{
			ImGui::Button("Save Entity");
			m_sSavedText = "Invalid Data";
		}
	}

	ImGui::SameLine();
	ImGui::Text(m_sSavedText.c_str());
#endif
}

void EntityEditor::SaveEntity()
{
#if _DEBUG
	m_vEntityData = m_vEntityDataCopy;

	JsonLoading::SaveJson(m_vEntityData, FOLDER_PATH + JsonFile);
	m_sSavedText = "Saved";
#endif
}
