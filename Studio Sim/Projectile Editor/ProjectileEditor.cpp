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
	// TODO: Initialise images files for sprites of projectiles.
}

#if _DEBUG
void ProjectileEditor::SpawnEditorWindow()
{
	if (ImGui::Begin("Projectile Editor", FALSE, ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_HorizontalScrollbar))
	{
		
	LoadPattern();
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
	
	m_bLoadButton = ImGui::Button("Load Pattern");
	ImGui::InputTextWithHint("##TileMapSaveFile", "New Save File Name", saveFileName, IM_ARRAYSIZE(saveFileName));
	ImGui::Text(m_sSelectedFile.c_str());

	if (!m_bLoadButton)
		return;
	
	if (FileLoading::OpenFileExplorer(m_sSelectedFile, m_sFilePath))
	{
		const size_t slash = m_sFilePath.find_last_of("/\\");
		m_sFilePath = m_sFilePath.substr(0, slash) + "\\" + saveFileName + ".json";
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