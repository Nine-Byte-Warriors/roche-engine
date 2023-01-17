#include "stdafx.h"
#include "ProjectileEditor.h"
#include "FileLoading.h"

#if _DEBUG
#include <imgui/imgui.h>
#endif // _DEBUG

#define DEFAULT_PATTERN_PATH "Resources\\Patterns\\"

ProjectileEditor::ProjectileEditor() :
	m_sSelectedFile("ProjectilePattern.json"),
	m_sFileContent(""),
	m_sFilePath("")
{
	m_pProjectileManager = std::make_shared<ProjectileManager>();
	// TODO: Initialise images files for sprites of projectiles.
}

void ProjectileEditor::Initialise(const Graphics& gfx, ConstantBuffer<Matrices>& mat)
{
	if (m_pProjectileManager == nullptr)
		return;

	// TODO: allow for multiples and nested managers
	m_pProjectileManager->Initialize(gfx, mat);
}

void ProjectileEditor::Update(const float dt)
{
	m_pProjectileManager->Update(dt);
}

void ProjectileEditor::Draw(ID3D11DeviceContext* context, XMMATRIX matrix)
{
	if (m_pProjectileManager == nullptr)
		return;

	// TODO: allow for multiples and nested managers
	m_pProjectileManager->Draw(context, matrix);
}

#if _DEBUG
void ProjectileEditor::SpawnEditorWindow()
{
	if (ImGui::Begin("Projectile Editor", FALSE, ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_HorizontalScrollbar))
	{
		LoadPattern();
		SavePattern();

		ShowPattern();

	//SaveToExistingFile();
	//SaveToNewFile();

	//SelectTileMapLayer();
	//DrawButton();

	//TileMapSelectionButtons();
	//TileMapSelectedText();
	//TileMapGridPreview();
	
	//ImGui::Text(std::string("Option 1").c_str());
	//ImGui::SameLine();
	////ImGui::InputText()
	//ImGui::Text(std::string("Option 2").c_str());
	//ImGui::Text(std::string("Option 3").c_str());
	////ImGui::Text(std::string("X: ").append(std::to_string(m_transform->GetPosition().x)).c_str());
	//ImGui::SameLine();
	////ImGui::Text(std::string("Y: ").append(std::to_string(m_transform->GetPosition().y)).c_str());

	//if (ImGui::Button("FIRE!"))
	//	EventSystem::Instance()->AddEvent(EVENTID::PlayerFire, nullptr);
	//
	}
	ImGui::End();
}

void ProjectileEditor::LoadPattern()
{
	std::ifstream fsPatternStream(DEFAULT_PATTERN_PATH + m_sSelectedFile);
	static char saveFileName[128] = "";
	
	bool bLoadButton = ImGui::Button("Load Pattern");
	ImGui::InputTextWithHint("##TileMapSaveFile", "New Save File Name", saveFileName, IM_ARRAYSIZE(saveFileName));
	ImGui::Text(m_sSelectedFile.c_str());

	if (!bLoadButton)
		return;
	
	if (FileLoading::OpenFileExplorer(m_sSelectedFile, m_sFilePath))
	{
		m_vecProjectiles.clear();
		JsonLoading::LoadJson(m_vecProjectiles, m_sFilePath);
		
		//json jData = JSONHealper::LoadJSON(m_sFilePath);
		ImGui::Text(m_sFilePath.append(" was loaded").c_str());
		//const size_t slash = m_sFilePath.find_last_of("/\\");
		//m_sFilePath = m_sFilePath.substr(0, slash) + "\\" + saveFileName + ".json";
		/*if (SaveWriteFile())
		{
			m_sSelectedFile = saveFileName;
			m_sSelectedFile += ".json";
			m_sSelectedFile += " Save Successful";
		}
		else
		{
			m_sSelectedFile = "Save Write Failed";
		}*/
	}
	else
	{
		m_sSelectedFile = "Open File Failed";
	}
}

void ProjectileEditor::SavePattern()
{
	static char saveFileName[128] = "";
	m_bSaveButton = ImGui::Button("Save Pattern");
	ImGui::InputTextWithHint("##TileMapSaveFile", "New Save File Name", saveFileName, IM_ARRAYSIZE(saveFileName));
	ImGui::Text(m_sSelectedFile.c_str());
	
	if (!m_bSaveButton)
		return;

	if (FileLoading::OpenFileExplorer(m_sSelectedFile, m_sFilePath))
	{
		// DEBUG: Fake Pattern
		//std::string m_sID;
		//std::string m_sName;
		//std::string m_sTexture;
		//int order;
		//float m_fSpeed;
		//float m_fLifeTime;
		
		ProjectileData::ProjectileJSON jData = 
		{
			"ProjectilePattern.json",
			"ProjectilePattern",
			"Resources\\Textures\\Projectile.png",
			1,
			100.0f,
			5.0f
		};
		
		std::vector<ProjectileData::ProjectileJSON> vecProjectileJSON;
		vecProjectileJSON.push_back(jData);
		
		JsonLoading::SaveJson(vecProjectileJSON, m_sFilePath);
	}
}

void ProjectileEditor::ShowPattern()
{
	std::string msg;

	msg = m_vecProjectiles.size() > 0
		? "Loaded Projectiles;"
		: "No Projectiles Loaded";
	ImGui::Text(msg.c_str());

	for (ProjectileData::ProjectileJSON& projectile : m_vecProjectiles)
	{
		ImGui::Separator();
		msg = "Pattern: " + projectile.m_sName;
		ImGui::Text(msg.c_str());

		msg = "ID: " + projectile.m_sID;
		ImGui::Text(msg.c_str());

		msg = "Speed: " + std::to_string(projectile.m_fSpeed);
		ImGui::Text(msg.c_str());

		ImGui::SliderFloat("Speed: ", &projectile.m_fSpeed, 0.0f, 100.0f, "%0.2f");
	}
}

void ProjectileEditor::AddToEvent() noexcept
{
	/*EventSystem::Instance()->AddClient(EVENTID::PlayerPosition, this);
	EventSystem::Instance()->AddClient(EVENTID::TargetPosition, this);
	EventSystem::Instance()->AddClient(EVENTID::PlayerFire, this);*/
}

void ProjectileEditor::HandleEvent(Event* event)
{
	switch (event->GetEventID())
	{
	/*case EVENTID::PlayerPosition:
		m_vSpawnPosition = *static_cast<Vector2f*>(event->GetData());
		break;
	case EVENTID::TargetPosition:
		m_vTargetPosition = *static_cast<Vector2f*>(event->GetData());
		break;
	case EVENTID::PlayerFire:
		SpawnProjectile();
		break;*/
	default:
		break;
	}
}
#endif