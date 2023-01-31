#include "stdafx.h"
#include "TileMapEditor.h"
#include "FileLoading.h"

#define FOLDER_PATH "Resources\\TileMaps\\"

TileMapEditor::TileMapEditor() { }

TileMapEditor::~TileMapEditor() { }

void TileMapEditor::Initialize(int rows, int columns)
{
	m_iRows = rows;
	m_iColumns = columns;

	m_tileMapBackground = std::make_shared<TileMap>(m_iRows, m_iColumns);
	m_tileMapForeground = std::make_shared<TileMap>(m_iRows, m_iColumns);

	m_sTileTypeData = m_tileMapBackground->GetTileTypeData();
	m_iSizeOfTileTypeData = m_sTileTypeData.size();

	m_iCurrentSelectedTileType = 0;
	m_sCurrentSelectedTileType = "Current Tile Type: ";
	m_sCurrentSelectedTileType += m_sTileTypeData[0].name;

	m_bDrawContinuous = true;
	m_bLayerSwitched = true;

	m_tileMapLayer = TileMapLayer::Background;
}

void TileMapEditor::SetJsonFile( const std::string& name )
{
	m_sFilePath = name;
	LoadProcessFile();
}

#if _DEBUG
void TileMapEditor::SpawnControlWindow()
{
	if (ImGui::Begin("Tile Map Editor", FALSE, ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_HorizontalScrollbar))
	{
		Load();
		SaveToExistingFile();
		SaveToNewFile();

		SelectTileMapLayer();
		DrawButton();

		TileMapSelectedText();
		TileMapSelectionButtons();
	}
	ImGui::End();
}
#endif

void TileMapEditor::SetMapDrawnDone()
{
	m_bMapUpdated = false;
}

void TileMapEditor::SetLayerSwitchedDone()
{
	m_bLayerSwitched = false;
}

void TileMapEditor::SetLoadedFileDone()
{
	m_bLoadedFile = false;
}

std::vector<int> TileMapEditor::GetUpdatedTileMapTiles()
{
	return m_iUpdatedTileMapTiles;
}

void TileMapEditor::SetClearUpdatedTileMapTiles()
{
	m_iUpdatedTileMapTiles.clear();
}

std::shared_ptr<TileMap> TileMapEditor::GetLevel(TileMapLayer layer)
{
	if (layer == TileMapLayer::Background)
	{
		return m_tileMapBackground;
	}
	else if (layer == TileMapLayer::Foreground)
	{
		return m_tileMapForeground;
	}
}

bool TileMapEditor::IsDrawContinuousAvalible()
{
	return m_bDrawContinuous && m_bMapUpdated;
}

bool TileMapEditor::IsLayerSwitched()
{
	return m_bLayerSwitched;
}

bool TileMapEditor::IsLoadedFile()
{
	return m_bLoadedFile;
}

void TileMapEditor::DrawButton()
{
#if _DEBUG
	ImGui::NewLine();
	ImGui::Checkbox("Draw Continuously", &m_bDrawContinuous);
#endif
}

void TileMapEditor::Load()
{
#if _DEBUG
	m_bSaveButton = ImGui::Button("Load");
	ImGui::Text(m_sSelectedFile.c_str());
#endif

	if (m_bSaveButton)
	{
		if (FileLoading::OpenFileExplorer(m_sSelectedFile, m_sFilePath))
		{
			if (LoadProcessFile())
			{
				m_bLoadedFile = true;
			}
			else
			{
				m_sSelectedFile = "Load Process Failed";
			}
		}
		else
		{
			m_sFilePath = m_sSelectedFile;
			m_sSelectedFile = "Open File Failed";
		}
	}
}

bool TileMapEditor::LoadProcessFile()
{
	std::vector<std::string> tileTypeName;
	JsonLoading::LoadJson(tileTypeName, m_sFilePath);

	if (tileTypeName.size() != m_iRows * m_iColumns)
	{
		return false;
	}

	for (int i = 0; i < m_iRows * m_iColumns; i++)
	{
		if (m_tileMapLayer == TileMapLayer::Background)
		{
			m_tileMapBackground->UpdateTileFromName(i, tileTypeName[i]);
		}
		else if (m_tileMapLayer == TileMapLayer::Foreground)
		{
			m_tileMapForeground->UpdateTileFromName(i, tileTypeName[i]);
		}
	}

	tileTypeName.clear();

	return true;
}

void TileMapEditor::SaveToExistingFile()
{
#if _DEBUG
	ImGui::NewLine();
	m_bSaveButton = ImGui::Button("Save To Existing File");
#endif

	if (m_bSaveButton)
	{
		if (FileLoading::OpenFileExplorer(m_sSelectedFile, m_sFilePath))
		{
			if (SaveWriteFile())
			{
				m_sSelectedFile += " Save Successful";
			}
			else
			{
				m_sFilePath = m_sSelectedFile;
				m_sSelectedFile = "Save Write Failed";
			}
		}
		else
		{
			m_sSelectedFile = "Open File Failed";
		}
	}
}

void TileMapEditor::SaveToNewFile()
{
#if _DEBUG
	m_bSaveNewButton = ImGui::Button("Save To New File");
	ImGui::InputTextWithHint("##TileMapSaveFile", "New Save File Name", m_cSaveFileName, IM_ARRAYSIZE(m_cSaveFileName));
#endif

	if (m_bSaveNewButton)
	{
		if (FileLoading::OpenFileExplorer(m_sSelectedFile, m_sFilePath))
		{
			const size_t slash = m_sFilePath.find_last_of("/\\");
			m_sFilePath = m_sFilePath.substr(0, slash) + "\\" + m_cSaveFileName + ".json";
			if (SaveWriteFile())
			{
				m_sSelectedFile = m_cSaveFileName;
				m_sSelectedFile += ".json";
				m_sSelectedFile += " Save Successful";
			}
			else
			{
				m_sSelectedFile = "Save Write Failed";
			}
		}
		else
		{
			m_sSelectedFile = "Open File Failed";
		}
	}
}

bool TileMapEditor::SaveWriteFile()
{
	std::vector<std::string> tileTypeName;

	for (int i = 0; i < m_iRows * m_iColumns; i++)
	{
		if (m_tileMapLayer == TileMapLayer::Background)
		{
			tileTypeName.push_back(m_tileMapBackground->GetTileTypeNameFromPosition(i));
		}
		else if (m_tileMapLayer == TileMapLayer::Foreground)
		{
			tileTypeName.push_back(m_tileMapForeground->GetTileTypeNameFromPosition(i));
		}
	}

	if (m_tileMapLayer == TileMapLayer::Background)
	{
		JsonLoading::SaveJson(tileTypeName, m_sFilePath);
	}
	else if (m_tileMapLayer == TileMapLayer::Foreground)
	{
		JsonLoading::SaveJson(tileTypeName, m_sFilePath);
	}

	tileTypeName.clear();

	return true;
}

void TileMapEditor::TileMapSelectionButtons()
{
#if _DEBUG
	ImGui::NewLine();

	for (int i = 0; i < m_iSizeOfTileTypeData; i++)
	{
		ImColor color = m_tileMapBackground->GetColorFromType(i);
		m_sTileTypeData[i].button =
			ImGui::ImageButtonNoTexture(m_sTileTypeData[i].name.c_str(), m_vImageButtonSize, m_vImageButtonFrame0, m_vImageButtonFrame1, m_iImageButtonPadding, color);

		ImGui::SameLine();
		ImGui::Text(m_sTileTypeData[i].name.c_str());
		if ((i + 1) % 4 != 0)
		{
			ImGui::SameLine();
		}
	}

	for (int i = 0; i < m_iSizeOfTileTypeData; i++)
	{
		if (m_sTileTypeData[i].button)
		{
			m_iCurrentSelectedTileType = m_sTileTypeData[i].type;
		}
	}
#endif
}

void TileMapEditor::TileMapSelectedText()
{
#if _DEBUG
	ImGui::NewLine();
#endif

	for (int i = 0; i < m_iSizeOfTileTypeData; i++)
	{
		if (m_iCurrentSelectedTileType == i)
		{
			m_sCurrentSelectedTileType = "Current Tile Type: ";
			m_sCurrentSelectedTileType += m_sTileTypeData[i].name;
		}
	}

#if _DEBUG
	ImGui::Text(m_sCurrentSelectedTileType.c_str());
#endif
}

void TileMapEditor::UpdateTileMap(int pos)
{
	m_bMapUpdated = true;
	m_iUpdatedTileMapTiles.push_back(pos);
	for (int j = 0; j < m_iSizeOfTileTypeData; j++)
	{
		if (m_iCurrentSelectedTileType == m_tileMapBackground->GetTileTypeData()[j].type)
		{
			if (m_tileMapLayer == TileMapLayer::Background)
			{
				m_tileMapBackground->UpdateTileFromType(pos, m_tileMapBackground->GetTileTypeData()[j].type);
			}
			else if (m_tileMapLayer == TileMapLayer::Foreground)
			{
				m_tileMapForeground->UpdateTileFromType(pos, m_tileMapBackground->GetTileTypeData()[j].type);
			}
		}
	}
}

void TileMapEditor::SelectTileMapLayer()
{
#if _DEBUG
	ImGui::NewLine();
	ImGui::Text("Select Tile Map Layer");
	const char* tileMapLayers[]{ "Background", "Foreground" };
	if (ImGui::BeginCombo("##SelectTileMapLayer", m_sPreviewMapLayer.c_str()))
	{
		for (int i = 0; i < IM_ARRAYSIZE(tileMapLayers); i++)
		{
			const bool isSelected = i == m_iTileMapLayer;
			if (ImGui::Selectable(tileMapLayers[i], isSelected))
			{
				m_iTileMapLayer = i;
				m_sPreviewMapLayer = tileMapLayers[i];
			}
		}
		ImGui::EndCombo();

		if (m_iTileMapLayer == 0)
		{
			m_tileMapLayer = TileMapLayer::Background;
		}
		else if (m_iTileMapLayer == 1)
		{
			m_tileMapLayer = TileMapLayer::Foreground;
		}
		m_bMapUpdated = true;
		m_bLayerSwitched = true;
	}
#endif
}

TileMapLayer TileMapEditor::GetTileMapLayer()
{
	return m_tileMapLayer;
}

int TileMapEditor::GetTileMapLayerInt()
{
	if (m_tileMapLayer == TileMapLayer::Background)
	{
		return 0;
	}
	else if (m_tileMapLayer == TileMapLayer::Foreground)
	{
		return 1;
	}
	return 2;
}

int TileMapEditor::GetTileMapOtherLayerInt()
{
	if (m_tileMapLayer == TileMapLayer::Background)
	{
		return 1;
	}
	else if (m_tileMapLayer == TileMapLayer::Foreground)
	{
		return 0;
	}
	return 2;
}

std::string TileMapEditor::GetTexture()
{
	m_sTexture = "Resources\\Textures\\Tiles\\" + m_sTileTypeData[m_iCurrentSelectedTileType].name + ".png";
	return m_sTexture;
}
