#include "stdafx.h"
#include "ProjectileEditor.h"
#include "FileHandler.h"

#include "Graphics.h"	// required for gfx initialisation

#if _DEBUG
#include <imgui/imgui.h>
#endif // _DEBUG

ProjectileEditor::ProjectileEditor() :
	m_sSelectedFile("ProjectilePattern.json"),
	m_sFileContent(""),
	m_sFilePath(""),
	m_iProjectileCount(0),
	m_bMidPosSet(false),
	m_vSpawnPosition(Vector2f())
{
}

void ProjectileEditor::Initialise(const Graphics& gfx, ConstantBuffer<Matrices>& mat)
{
	for(std::shared_ptr<ProjectileManager> pProMan : m_vecProjectileManager)
		pProMan->Initialize(gfx, mat);
}

void ProjectileEditor::Update(const float dt)
{
	for (std::shared_ptr <ProjectileManager> pProMan : m_vecProjectileManager)
		pProMan->Update(dt);
}

void ProjectileEditor::Draw(ID3D11DeviceContext* context, XMMATRIX matrix)
{
	for (std::shared_ptr <ProjectileManager> pProMan : m_vecProjectileManager)
		pProMan->Draw(context, matrix);
}

#if _DEBUG
void ProjectileEditor::SpawnEditorWindow(const Graphics& gfx, ConstantBuffer<Matrices>& mat)
{
	if (ImGui::Begin("Projectile Editor", FALSE, ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_HorizontalScrollbar))
	{
		LoadPattern();
		SavePattern();

		ImGui::Separator();
		SpawnPosition(Vector2f(gfx.GetWidth(), gfx.GetHeight()));

		ImGui::Separator();
		ShowPattern();

		TestButtons(gfx, mat);
	}
	ImGui::End();
}

void ProjectileEditor::LoadPattern()
{
	bool bLoadButton = ImGui::Button("Load Pattern");
	
	static char loadFileName[128] = "";
	ImGui::InputTextWithHint("##PatternLoadFile", "Load File Name", loadFileName, IM_ARRAYSIZE(loadFileName));

	if (!bLoadButton)
		return;
	
	// Call the FileDialog Builder and store the result in a file object.
	std::shared_ptr<FileHandler::FileObject>foLoad = FileHandler::FileDialog(foLoad)
		->UseOpenDialog()	// Choose the dialog to use.
		->ShowDialog()		// Show the dialog.
		->StoreDialogResult();	// Store the result.

	// Check if the file object has a file path/name.
	if (foLoad->HasPath())
	{
		JsonLoading::LoadJson(m_vecManagers, foLoad->GetFullPath());
		if (m_vecManagers.size() == 0)
		{
			m_sSelectedFile = "Open File Failed";
			return;
		}
	}
	else
		m_sSelectedFile = "Open File Failed";

	// Smile. :D
}

void ProjectileEditor::SavePattern()
{
	m_bSaveButton = ImGui::Button("Save Pattern");
	
	static char saveFileName[128] = "";
	ImGui::InputTextWithHint("##PatternSaveFile", "New Save File Name", saveFileName, IM_ARRAYSIZE(saveFileName));

	if (!m_bSaveButton)
		return;
	
	if (m_vecManagers.size() < 1)
		return;
	
	// Create a file object with a file name. Optional.
	std::shared_ptr<FileHandler::FileObject> foSave = FileHandler::CreateFileObject(saveFileName);
	
	// pass file object to the FileDialog Buidler.
	foSave = FileHandler::FileDialog(foSave)
		->UseSaveDialog()	// Choose the dialog to use.
		->ShowDialog()		// Show the dialog.
		->StoreDialogResult();	// Store the result.
	
	// Check if the file object has a file path/name.
	if (foSave->HasPath())
		// Save the file.
		JsonLoading::SaveJson(m_vecManagers, foSave->GetJsonPath());
	
	// Smile. :D
}

void ProjectileEditor::SpawnPosition(Vector2f vWinMax)
{
	if (!m_bMidPosSet)
	{
		m_vSpawnPosition = Vector2f(vWinMax.x / 2, vWinMax.y / 2);
		m_bMidPosSet = true;
	}

	ImGui::Text("Spawn Position");
	ImGui::DragFloat("SpawnPosition-X", &m_vSpawnPosition.x, 1.0f, 0.0f, vWinMax.x, "%.1f");
	ImGui::DragFloat("SpawnPosition-Y", &m_vSpawnPosition.y, 1.0f, 0.0f, vWinMax.y, "%.1f");
}

void ProjectileEditor::ShowPattern()
{
	std::string msg;
	
	if (m_vecManagers.size() < 1)
		m_vecManagers.push_back(CreateDefaultManager());
	
	if(ImGui::Button("Add Manager"))
		m_vecManagers.push_back(CreateDefaultManager());

	for (int iManIndex = 0; iManIndex < m_vecManagers.size(); iManIndex++)
	{
		std::string sManagerTitle = std::string("Manager #").append(std::to_string(iManIndex));
		if(ImGui::CollapsingHeader(sManagerTitle.c_str()))
		{
			msg = "Del Manager #" + std::to_string(iManIndex) + " ##Man" + std::to_string(iManIndex);
			if (ImGui::Button(msg.c_str()))
			{
				m_vecManagers.erase(m_vecManagers.begin() + iManIndex);
				break;
			}

			msg = "Pattern " + m_vecManagers[iManIndex].m_sName;
			ImGui::Text(msg.c_str());

			msg = "ID: " + m_vecManagers[iManIndex].m_sID;
			ImGui::Text(msg.c_str());

			ImGui::Text("Projectile Size: ");
			static float fSize[2];
			fSize[0] = m_vecManagers[iManIndex].m_fWidth;
			fSize[1] = m_vecManagers[iManIndex].m_fHeight;
			ImGui::SliderFloat(
				std::string("Width##Man")
					.append(std::to_string(iManIndex))
					.c_str(),
				&m_vecManagers[iManIndex].m_fWidth, 0.1f, 256.0f);
			ImGui::SliderFloat(
				std::string("Height##Man")
					.append(std::to_string(iManIndex))
					.c_str(),
				&m_vecManagers[iManIndex].m_fHeight, 0.1f, 256.0f);

			ImGui::Checkbox(
				std::string("Use Global Speed##Man")
					.append(std::to_string(iManIndex))
					.c_str(),
				&m_vecManagers[iManIndex].m_bUseGlobalSpeed);

			if (m_vecManagers[iManIndex].m_bUseGlobalSpeed == true)
			{
				ImGui::SliderFloat(
					std::string("Global Speed##Man")
						.append(std::to_string(iManIndex))
						.c_str(),
					&m_vecManagers[iManIndex].m_fGlobalSpeed, -100.0f, 100.0f);
			}
			
			ImGui::Text(std::string("Count: ").append(std::to_string(m_vecManagers[iManIndex].m_vecProjectiles.size())).c_str());
			
			ImGui::Text(std::string("Delay: ").append(std::to_string(m_vecManagers[iManIndex].m_fDelay)).c_str());
			ImGui::SliderFloat(
				std::string("Delay##").append(std::to_string(iManIndex)).c_str(),
				&m_vecManagers[iManIndex].m_fDelay,
				0, 100
			);

			for (int iProIndex = 0; iProIndex < m_vecManagers[iManIndex].m_vecProjectiles.size(); iProIndex++)
			{
				std::string sProjTitle = "Projectile #";
				sProjTitle
					.append(std::to_string(iProIndex))
					.append("##Man")
					.append(std::to_string(iManIndex));
				if (ImGui::TreeNode(sProjTitle.c_str()))
				{
					ImGui::Text("Spawn Offset");
					ImGui::SliderFloat(
						std::string("X##Man")
							.append(std::to_string(iManIndex))
							.append("Pro")
							.append(std::to_string(iProIndex))
							.c_str(),
						&m_vecManagers[iManIndex].m_vecProjectiles[iProIndex].m_fX,
						-100.0f, 100.0f);
					ImGui::SliderFloat(
						std::string("Y##Man")
							.append(std::to_string(iManIndex))
							.append("Pro")
							.append(std::to_string(iProIndex))
							.c_str(),
						&m_vecManagers[iManIndex].m_vecProjectiles[iProIndex].m_fY,
						-100.0f, 100.0f);

					ImGui::Separator();

					msg = "Delay: " + std::to_string(m_vecManagers[iManIndex].m_vecProjectiles[iProIndex].m_fDelay);
					ImGui::Text(msg.c_str()); 
					ImGui::SliderFloat(
						std::string("Delay##Man")
							.append(std::to_string(iManIndex))
							.append("Pro")
							.append(std::to_string(iProIndex))
							.c_str(),
						&m_vecManagers[iManIndex].m_vecProjectiles[iProIndex].m_fDelay,
						-100, 100, "%0.2f");

					msg = "LifeTime: " + std::to_string(m_vecManagers[iManIndex].m_vecProjectiles[iProIndex].m_fLifeTime);
					ImGui::Text(msg.c_str());
					ImGui::SliderFloat(
						std::string("LifeTime##Man")
							.append(std::to_string(iManIndex))
							.append("Pro")
							.append(std::to_string(iProIndex))
							.c_str(),
						&m_vecManagers[iManIndex].m_vecProjectiles[iProIndex].m_fLifeTime,
						0.0f, 1000.0f, "%0.2f");

					msg = "Speed: " + std::to_string(m_vecManagers[iManIndex].m_vecProjectiles[iProIndex].m_fSpeed);
					ImGui::Text(msg.c_str());
					ImGui::SliderFloat(
						std::string("Speed##Man")
							.append(std::to_string(iManIndex))
							.append("Pro")
							.append(std::to_string(iProIndex))
							.c_str(), 
						&m_vecManagers[iManIndex].m_vecProjectiles[iProIndex].m_fSpeed,
						0.0f, 100.0f, "%0.2f");

					msg = "Heading Angle: " + std::to_string(m_vecManagers[iManIndex].m_vecProjectiles[iProIndex].m_fAngle);
					ImGui::Text(msg.c_str());
					ImGui::SliderAngle(
						std::string("Angle##Man")
							.append(std::to_string(iManIndex))
							.append("Pro")
							.append(std::to_string(iProIndex))
							.c_str(),
						&m_vecManagers[iManIndex].m_vecProjectiles[iProIndex].m_fAngle,-360.0f,360.0f,"%.2f deg");
					
					msg = "Amplitude: " + std::to_string(m_vecManagers[iManIndex].m_vecProjectiles[iProIndex].m_fAmplitude);
					ImGui::Text(msg.c_str());
					ImGui::DragFloat(
						std::string("Amplitude##Man")
							.append(std::to_string(iManIndex))
							.append("Pro")
							.append(std::to_string(iProIndex))
							.c_str(),
						&m_vecManagers[iManIndex].m_vecProjectiles[iProIndex].m_fAmplitude,
						0.1f, -100.0f, 100.0f, "%.2f");

					msg = "Frequency: " + std::to_string(m_vecManagers[iManIndex].m_vecProjectiles[iProIndex].m_fFrequency);
					ImGui::Text(msg.c_str());
					ImGui::DragFloat(
						std::string("Frequency##Man")
							.append(std::to_string(iManIndex))
							.append("Pro")
							.append(std::to_string(iProIndex))
							.c_str(),
						&m_vecManagers[iManIndex].m_vecProjectiles[iProIndex].m_fFrequency,
						0.1f, -100.0f, 100.0f, "%.2f");

					ImGui::TreePop();
				}
			
				msg = "Del Projectile #" + std::to_string(iProIndex) + " ##Man" + std::to_string(iManIndex) + "Pro" + std::to_string(iProIndex);
				if (ImGui::Button(msg.c_str()))
					m_vecManagers[iManIndex].m_vecProjectiles.erase(m_vecManagers[iManIndex].m_vecProjectiles.begin() + iProIndex);
			}

			if (ImGui::Button(std::string("Add Projectile##Man").append(std::to_string(iManIndex)).c_str()))
				m_vecManagers[iManIndex].m_vecProjectiles.push_back(CreateDefaultProjectile());
		}
	}
}

void ProjectileEditor::TestButtons(const Graphics& gfx, ConstantBuffer<Matrices>& mat)
{
	bool bFire = ImGui::Button("Test Fire");
	bool bLoop = ImGui::Button("Test Loop");
	// TODO: once all projectiles have been spawned, reset and respawn

	if (!bFire)
		return;

	m_vecProjectileManager.clear();

	for (int i = 0; i < m_vecManagers.size(); i++)
	{
		std::shared_ptr <ProjectileManager> pManager = std::make_shared<ProjectileManager>(); 
		pManager->SetDelay(m_vecManagers[i].m_fDelay);
		pManager->SetProjectilePool(CreateProjectilePool(
				m_vecManagers[i].m_vecProjectiles, 
				m_vecManagers[i].m_fGlobalSpeed, 
				m_vecManagers[i].m_bUseGlobalSpeed)
		);

		Vector2f vSize = Vector2f(m_vecManagers[i].m_fWidth, m_vecManagers[i].m_fHeight);
		pManager->InitialiseFromFile(gfx, mat, m_vecManagers[i].m_sImagePath, vSize);

		m_vecProjectileManager.push_back(std::move(pManager)); 
	}	

	SpawnPattern();
}

std::vector<std::shared_ptr<Projectile>> ProjectileEditor::CreateProjectilePool(std::vector<ProjectileData::ProjectileJSON> vecProjectileJsons, float fGlobalSpeed, bool bUseGlobalSpeed)
{
	std::vector<std::shared_ptr<Projectile>> vecProjectilePool;

	for (ProjectileData::ProjectileJSON pJson : vecProjectileJsons)
	{
		std::shared_ptr<Projectile> pProjectile = std::make_shared<Projectile>(
			bUseGlobalSpeed == true
			? fGlobalSpeed
			: pJson.m_fSpeed,
			pJson.m_fLifeTime
		);
		pProjectile->SetDirection(Vector2f(pJson.m_fAngle));
		pProjectile->SetOffSet(Vector2f(pJson.m_fX, pJson.m_fY));
		pProjectile->SetWave(pJson.m_fAngle, pJson.m_fAmplitude, pJson.m_fFrequency);
		pProjectile->SetDelay(pJson.m_fDelay);

		vecProjectilePool.push_back(std::move(pProjectile));
	}

	return vecProjectilePool;
}

void ProjectileEditor::SaveProjectile()
{
	JsonLoading::SaveJson(m_vecProjectiles, m_sFilePath);
}

void ProjectileEditor::SpawnPattern()
{
	for (std::shared_ptr<ProjectileManager> manager : m_vecProjectileManager)
		manager->SpawnProjectiles(m_vSpawnPosition);
}

ProjectileData::ManagerJSON ProjectileEditor::CreateDefaultManager()
{
	ProjectileData::ManagerJSON manager;
	manager.m_sID = "ID";
	manager.m_sName = "Default Pattern";
	manager.m_sImagePath = "Resources\\Textures\\Base_Projectile.png";
	manager.m_fWidth = 0.0f;
	manager.m_fHeight = 0.0f;
	manager.m_fDelay = 0.0f;
	manager.m_iCount = 1;
	manager.m_bLoop = false;
	manager.m_bUseGlobalSpeed = false;
	manager.m_fGlobalSpeed = 0.0f;

	manager.m_vecProjectiles.push_back(CreateDefaultProjectile());
	
	return manager;
}

ProjectileData::ProjectileJSON ProjectileEditor::CreateDefaultProjectile()
{
	ProjectileData::ProjectileJSON blankProjectile{};

	blankProjectile.m_fSpeed = 10.0f;
	blankProjectile.m_fLifeTime = 100;
	blankProjectile.m_fX = 0.0f;
	blankProjectile.m_fY = 0.0f;
	blankProjectile.m_fDelay = 0.0f;
	blankProjectile.m_fAngle = 0.0f;
	blankProjectile.m_fAmplitude = 0.0f;
	blankProjectile.m_fFrequency = 0.0f;
	
	return blankProjectile;
}
#endif // _DEBUG