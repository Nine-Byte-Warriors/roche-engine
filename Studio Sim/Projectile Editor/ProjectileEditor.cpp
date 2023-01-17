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
	m_sFilePath(""),
	m_iProjectileCount(0),
	m_vSpawnPosition(Vector2f())
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
void ProjectileEditor::SpawnEditorWindow(const Graphics& gfx, ConstantBuffer<Matrices>& mat)
{
	if (ImGui::Begin("Projectile Editor", FALSE, ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_HorizontalScrollbar))
	{
		LoadPattern();
		SavePattern();

		ShowPattern();

		TestButtons(gfx, mat);
		SpawnPattern();
	}
	ImGui::End();
}

void ProjectileEditor::LoadPattern()
{
	static char saveFileName[128] = "";
	
	bool bLoadButton = ImGui::Button("Load Pattern");
	ImGui::InputTextWithHint("##TileMapSaveFile", "New Save File Name", saveFileName, IM_ARRAYSIZE(saveFileName));
	ImGui::Text(m_sSelectedFile.c_str());

	if (!bLoadButton)
		return;
	
	if (FileLoading::OpenFileExplorer(m_sSelectedFile, m_sFilePath))
	{
		//m_vecProjectiles.clear();
		JsonLoading::LoadJson(m_vecProjectiles, m_sFilePath);
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
		SaveProjectile();
	}
}

void ProjectileEditor::ShowPattern()
{
	std::string msg;

	msg = m_vecProjectiles.size() > 0
		? "Loaded Projectiles;"
		: "No Projectiles Loaded";
	ImGui::Text(msg.c_str());

	if (m_vecProjectiles.size() < 1)
	{
		ProjectileData::ProjectileJSON blank;
		blank.m_fAngle = 0.0f;
		blank.m_fLifeTime = 100;
		blank.m_fSpeed = 10.0f;
		blank.m_fX = 0.0f;
		blank.m_fY = 0.0f;
		blank.m_iCount = 0;
		blank.m_iOrder = 0;
		blank.m_sID = "Default";
		blank.m_sName = "Default";
		blank.m_sTexture = "Resources\\Textures\\Base_Projectile.png";
		m_vecProjectiles.push_back(blank);
	}

	for (ProjectileData::ProjectileJSON& projectile : m_vecProjectiles)
	{
		ImGui::Separator();
	
		msg = "Pattern: " + projectile.m_sName;
		ImGui::Text(msg.c_str());

		msg = "ID: " + projectile.m_sID;
		ImGui::Text(msg.c_str());

		msg = "Count: " + std::to_string(projectile.m_iCount);
		ImGui::Text(msg.c_str());
		ImGui::SliderInt("Count", &projectile.m_iCount, 0, 100);

		msg = "Order: " + std::to_string(projectile.m_iOrder);
		ImGui::Text(msg.c_str());
		ImGui::SliderInt("Order", &projectile.m_iOrder, 0, 100);

		msg = "LifeTime: " + std::to_string(projectile.m_fLifeTime);
		ImGui::Text(msg.c_str());
		ImGui::SliderFloat("LifeTime: ", &projectile.m_fLifeTime, 0.0f, 1000.0f, "%0.2f");

		msg = "Speed: " + std::to_string(projectile.m_fSpeed);
		ImGui::Text(msg.c_str());
		ImGui::SliderFloat("Speed: ", &projectile.m_fSpeed, 0.0f, 100.0f, "%0.2f");

		ImGui::Text("Spawn Position");
		ImGui::SliderFloat("X", &projectile.m_fX, -100.0f, 100.0f);
		ImGui::SliderFloat("Y", &projectile.m_fY, -100.0f, 100.0f);

		ImGui::Separator();
		msg = "Angle: " + std::to_string(projectile.m_fAngle);
		ImGui::SliderAngle("Angle:", &projectile.m_fAngle);
	}
}

void ProjectileEditor::TestButtons(const Graphics& gfx, ConstantBuffer<Matrices>& mat)
{
	bool bFire = ImGui::Button("Test Fire");

	if (!bFire)
		return;

	// TODO: manage multiple managers
	if (m_vecProjectiles.size() > 0)
		m_pProjectileManager->ResetPool(m_vecProjectiles[0], gfx, mat);
}

void ProjectileEditor::SaveProjectile()
{
	JsonLoading::SaveJson(m_vecProjectiles, m_sFilePath);
}

void ProjectileEditor::SpawnPattern()
{
	if (m_vecProjectiles.size() < 1)
		return;

	for (ProjectileData::ProjectileJSON projectile : m_vecProjectiles)
	{
		Vector2f vSpawnPosition = Vector2f(projectile.m_fX, projectile.m_fY);
		m_pProjectileManager->SpawnProjectile(vSpawnPosition, projectile.m_fAngle);
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