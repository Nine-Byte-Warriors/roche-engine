#include "stdafx.h"
#include "TileMapEditor.h"
#include "FileLoading.h"

TileMapEditor::TileMapEditor(int rows, int columns)
{
	m_iRows = rows;
	m_iColumns = columns;

	m_tileMapBackground = new TileMap(m_iRows, m_iColumns);
	m_tileMapForeground = new TileMap(m_iRows, m_iColumns);

	m_sTileTypeData = m_tileMapBackground->GetTileTypeData();
	m_iSizeOfTileTypeData = m_sTileTypeData.size();

#if _DEBUG
	for (int i = 0; i < m_iRows * m_iColumns; i++)
	{
		m_TileMapPreviewImageButtonColor.push_back(m_sTileTypeData[0].color);
	}
#endif

	m_iCurrentSelectedTileType = 0;
	m_sCurrentSelectedTileType = "Current Tile Type: ";
	m_sCurrentSelectedTileType += m_sTileTypeData[0].name;

	m_bDrawOnce = false;
	m_bDrawContinuous = false;

	m_tileMapLayer = TileMapLayer::Background;
}

TileMapEditor::~TileMapEditor()
{
	delete m_tileMapBackground;
	delete m_tileMapForeground;
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

		TileMapSelectionButtons();
		TileMapSelectedText();
		TileMapGridPreview();

	}

	ImGui::End();
}
#endif

bool TileMapEditor::UpdateDrawOnceAvalible()
{
	return m_bDrawOnce;
}

void TileMapEditor::UpdateDrawOnceDone()
{
	m_bDrawOnce = false;
}

bool TileMapEditor::UpdateDrawContinuousAvalible()
{
	return m_bDrawContinuous;
}

std::string TileMapEditor::GetTileTypeName(int pos, TileMapLayer tileMapLayer)
{
	if (tileMapLayer == TileMapLayer::Background)
	{
		return m_sTileTypeData[m_tileMapBackground->GetTileType(pos)].name;
	}
	else if (tileMapLayer == TileMapLayer::Foreground)
	{
		return m_sTileTypeData[m_tileMapForeground->GetTileType(pos)].name;
	}
}

void TileMapEditor::DrawButton()
{
#if _DEBUG
	ImGui::NewLine();
	m_bDrawButton = ImGui::Button("Draw Once");
	if (m_bDrawButton)
	{
		m_bDrawOnce = true;
	}
	ImGui::Checkbox("Draw Continuously", &m_bDrawContinuous);
#endif
}

void TileMapEditor::Load()
{
#if _DEBUG
	m_bLoadButton = ImGui::Button("Load");
	ImGui::Text(m_sSelectedFile.c_str());
#endif

	if (m_bLoadButton)
	{
		if (m_tileMapLayer != TileMapLayer::Both)
		{
			if (FileLoading::OpenFileExplorer(m_sSelectedFile, m_sFilePath))
			{
				if (!LoadProcessFile())
				{
					m_sSelectedFile = "Load Process Failed";
				}
			}
			else
			{
				m_sSelectedFile = "Open File Failed";
			}
		}
		else
		{
			m_sSelectedFile = "Can't Load Both";
		}
	}
}

bool TileMapEditor::LoadProcessFile()
{
	if (m_tileMapLayer == TileMapLayer::Both)
	{
		return false;
	}

	std::ifstream file(m_sFilePath);
	json jsonFile;
	json jsonRow;
	file >> jsonFile;
	int rowNum = 0;
	int colNum = 0;
	std::string rowNumStr;

	for (int i = 0; i < m_iRows; i++)
	{
		rowNumStr = "row";
		rowNumStr += std::to_string(rowNum);
		jsonRow = jsonFile[rowNumStr];

		for (auto jsonTileType = jsonRow.begin(); jsonTileType != jsonRow.end(); ++jsonTileType)
		{
			for (int tileType = 0; tileType < m_iSizeOfTileTypeData; tileType++)
			{
				if (jsonTileType.value() == std::to_string(tileType) || StringHelper::StringEqualsIgnoreCase(jsonTileType.value(), m_sTileTypeData[tileType].name))
				{
					int pos = rowNum * m_iColumns + colNum;
#if _DEBUG
					m_TileMapPreviewImageButtonColor[pos] = m_sTileTypeData[tileType].color;
#endif
					if (m_tileMapLayer == TileMapLayer::Background)
					{
						m_tileMapBackground->UpdateTile(pos, tileType);
					}
					else if (m_tileMapLayer == TileMapLayer::Foreground)
					{
						m_tileMapForeground->UpdateTile(pos, tileType);
					}
				}
			}
			colNum++;
		}
		colNum = 0;
		rowNum++;
	}
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
		if (m_tileMapLayer != TileMapLayer::Both)
		{
			if (FileLoading::OpenFileExplorer(m_sSelectedFile, m_sFilePath))
			{
				if (SaveWriteFile())
				{
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
		else
		{
			m_sSelectedFile = "Can't Save Both";
		}

	}
}

void TileMapEditor::SaveToNewFile()
{
	static char saveFileName[128] = "";
#if _DEBUG
	m_bSaveNewButton = ImGui::Button("Save To New File");
	ImGui::InputTextWithHint("##TileMapSaveFile", "New Save File Name", saveFileName, IM_ARRAYSIZE(saveFileName));
#endif

	if (m_bSaveNewButton)
	{
		if (m_tileMapLayer != TileMapLayer::Both)
		{
			if (FileLoading::OpenFileExplorer(m_sSelectedFile, m_sFilePath))
			{
				const size_t slash = m_sFilePath.find_last_of("/\\");
				m_sFilePath = m_sFilePath.substr(0, slash) + "\\" + saveFileName + ".json";
				if (SaveWriteFile())
				{
					m_sSelectedFile = saveFileName;
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
		else
		{
			m_sSelectedFile = "Can't Save Both";
		}

	}
}

bool TileMapEditor::SaveWriteFile()
{
	json jsonFile;
	int rowNum;
	std::string rowNumStr;
	std::vector<std::string> rowValues;

	for (int i = 0; i < m_iRows * m_iColumns; i++)
	{
		if (m_tileMapLayer == TileMapLayer::Background)
		{
			rowValues.push_back(m_sTileTypeData[m_tileMapBackground->GetTileType(i)].name);
		}
		else if (m_tileMapLayer == TileMapLayer::Foreground)
		{
			rowValues.push_back(m_sTileTypeData[m_tileMapForeground->GetTileType(i)].name);
		}

		bool endOfRow = (i + 1) % m_iColumns == 0 && i != 0;
		if (endOfRow)
		{
			rowNum = i / m_iColumns;
			rowNumStr = "row";
			rowNumStr += std::to_string(rowNum);
			jsonFile[rowNumStr] = rowValues;
			rowValues.clear();
		}
	}

	std::ofstream o(m_sFilePath);
	o << std::setw(4) << jsonFile << std::endl;

	return true;
}

void TileMapEditor::TileMapSelectionButtons()
{
#if _DEBUG
	ImGui::NewLine();

	for (int i = 0; i < m_iSizeOfTileTypeData; i++)
	{
		m_sTileTypeData[i].button =
			ImGui::ImageButtonNoTexture(m_sTileTypeData[i].name.c_str(), m_vImageButtonSize, m_vImageButtonFrame0, m_vImageButtonFrame1, m_iImageButtonPadding, m_sTileTypeData[i].color);

		ImGui::SameLine();
		ImGui::Text(m_sTileTypeData[i].name.c_str());
		if ((i + 1) % (m_iColumns / 2) != 0)
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
	ImGui::Text("Left click to single edit; hold right click to multi edit");
#endif
}

void TileMapEditor::TileMapGridInit()
{
#if _DEBUG
	for (int i = 0; i < m_iRows * m_iColumns; i++)
	{
		std::string gridLable = "Grid" + std::to_string(i);
		m_bTileMapPreviewImageButton.push_back(ImGui::ImageButtonNoTexture(gridLable.c_str(), m_vImageButtonSize, m_vImageButtonFrame0, m_vImageButtonFrame1, m_iImageButtonPadding, m_TileMapPreviewImageButtonColor[i]));
	}
#endif
}

void TileMapEditor::TileMapGridPreview()
{
#if _DEBUG
	ImGui::NewLine();

	static bool firstTime = true;
	if (firstTime)
	{
		TileMapGridInit();
		firstTime = false;
	}

	for (int i = 0; i < m_iRows * m_iColumns; i++)
	{
		std::string gridLable = "Grid" + std::to_string(i);
		m_bTileMapPreviewImageButton[i] = ImGui::ImageButtonNoTexture(gridLable.c_str(), m_vImageButtonSize, m_vImageButtonFrame0, m_vImageButtonFrame1, m_iImageButtonPadding, m_TileMapPreviewImageButtonColor[i]);
		if ((i % m_iColumns) - (m_iColumns - 1) != 0)
		{
			ImGui::SameLine();
		}
	}
#endif

	UpdateSingleTileMapGridPreview();
}

void TileMapEditor::UpdateSingleTileMapGridPreview()
{
	if (m_tileMapLayer != TileMapLayer::Both)
	{
		for (int i = 0; i < m_iRows * m_iColumns; i++)
		{
			if (m_bTileMapPreviewImageButton[i])
			{
				for (int j = 0; j < m_iSizeOfTileTypeData; j++)
				{
					if (m_iCurrentSelectedTileType == m_tileMapBackground->GetTileTypeData()[j].type)
					{
#if _DEBUG
						m_TileMapPreviewImageButtonColor[i] = m_sTileTypeData[j].color;
#endif
						if (m_tileMapLayer == TileMapLayer::Background)
						{
							m_tileMapBackground->UpdateTile(i, m_tileMapBackground->GetTileTypeData()[j].type);
						}
						else if (m_tileMapLayer == TileMapLayer::Foreground)
						{
							m_tileMapForeground->UpdateTile(i, m_tileMapBackground->GetTileTypeData()[j].type);
						}
					}
				}
			}
		}
	}
}

void TileMapEditor::UpdateWholeTileMapGridPreview()
{
#if _DEBUG
	for (int i = 0; i < m_iRows * m_iColumns; i++)
	{
		for (int j = 0; j < m_iSizeOfTileTypeData; j++)
		{
			if (m_tileMapLayer == TileMapLayer::Background)
			{
				if (m_tileMapBackground->GetTileType(i) == m_tileMapBackground->GetTileTypeData()[j].type)
				{
					m_TileMapPreviewImageButtonColor[i] = m_sTileTypeData[j].color;
				}
			}
			else if (m_tileMapLayer == TileMapLayer::Foreground)
			{
				if (m_tileMapForeground->GetTileType(i) == m_tileMapForeground->GetTileTypeData()[j].type)
				{
					m_TileMapPreviewImageButtonColor[i] = m_sTileTypeData[j].color;
				}
			}
		}
	}
#endif
}

void TileMapEditor::SelectTileMapLayer()
{
#if _DEBUG
	ImGui::NewLine();
	ImGui::Text("Select Tile Map Layer");
	static int tileMapLayer = 0;
	static std::string previewMapLayer = "Background";
	static const char* tileMapLayers[]{ "Background", "Foreground", "Both - Editor Locked"};
	if (ImGui::BeginCombo("##SelectTileMapLayer", previewMapLayer.c_str()))
	{
		for (int i = 0; i < IM_ARRAYSIZE(tileMapLayers); i++)
		{
			const bool isSelected = i == tileMapLayer;
			if (ImGui::Selectable(tileMapLayers[i], isSelected))
			{
				tileMapLayer = i;
				previewMapLayer = tileMapLayers[i];
			}
		}
		ImGui::EndCombo();

		if (tileMapLayer == 0)
		{
			m_tileMapLayer = TileMapLayer::Background;
			UpdateWholeTileMapGridPreview();
		}
		else if (tileMapLayer == 1)
		{
			m_tileMapLayer = TileMapLayer::Foreground;
			UpdateWholeTileMapGridPreview();
		}
		else
		{
			m_tileMapLayer = TileMapLayer::Both;
		}
	}
#endif
}

TileMapLayer TileMapEditor::GetTileMapLayer()
{
	return m_tileMapLayer;
}
