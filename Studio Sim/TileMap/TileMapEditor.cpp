#include "stdafx.h"
#include "TileMapEditor.h"

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
		if (OpenFileExplorer())
		{
			if (LoadReadFile())
			{
				if (!LoadProcessFile())
				{
					m_sSelectedFile = "Can't load to both";
				}
			}
			else
			{
				m_sSelectedFile = "Read File Failed";
			}
		}
		else
		{
			m_sSelectedFile = "Open File Failed";
		}
	}
}

bool TileMapEditor::OpenFileExplorer()
{
	HRESULT f_SysHr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	//if (FAILED(f_SysHr))
	//	return FALSE;

	// CREATE FileOpenDialog OBJECT
	IFileOpenDialog* f_FileSystem;
	f_SysHr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&f_FileSystem));
	if (FAILED(f_SysHr)) {
		CoUninitialize();
		return FALSE;
	}

	//  SHOW OPEN FILE DIALOG WINDOW
	f_SysHr = f_FileSystem->Show(NULL);
	if (FAILED(f_SysHr)) {
		f_FileSystem->Release();
		CoUninitialize();
		return FALSE;
	}

	//  RETRIEVE FILE NAME FROM THE SELECTED ITEM
	IShellItem* f_Files;
	f_SysHr = f_FileSystem->GetResult(&f_Files);
	if (FAILED(f_SysHr)) {
		f_FileSystem->Release();
		CoUninitialize();
		return FALSE;
	}

	//  STORE AND CONVERT THE FILE NAME
	PWSTR f_Path;
	f_SysHr = f_Files->GetDisplayName(SIGDN_FILESYSPATH, &f_Path);
	if (FAILED(f_SysHr)) {
		f_Files->Release();
		f_FileSystem->Release();
		CoUninitialize();
		return FALSE;
	}

	//  FORMAT AND STORE THE FILE PATH
	std::wstring path(f_Path);
	std::string c(path.begin(), path.end());
	m_sFilePath = c;

	//  FORMAT STRING FOR EXECUTABLE NAME
	const size_t slash = m_sFilePath.find_last_of("/\\");
	m_sSelectedFile = m_sFilePath.substr(slash + 1);

	//  SUCCESS, CLEAN UP
	CoTaskMemFree(f_Path);
	f_Files->Release();
	f_FileSystem->Release();
	CoUninitialize();
}

bool TileMapEditor::LoadReadFile()
{
	std::string line;
	std::ifstream myFile;
	myFile.open(m_sFilePath);
	if (myFile.is_open())
	{
		m_sFileContent = "";
		while (getline(myFile, line))
		{
			m_sFileContent += line;
		}
		myFile.close();
		return true;
	}
	else
	{
		return false;
	}
}

bool TileMapEditor::LoadProcessFile()
{
	if (m_tileMapLayer == TileMapLayer::Both)
	{
		return false;
	}
	std::vector<std::string> word;
	
	for (int i = 0; i < m_iRows * m_iColumns; i++)
	{
		word.push_back("");
	}	

	int pos = 0;

	for (char& c : m_sFileContent)
	{
		if (c == ',')
		{
			pos++;
		}
		else
		{
			word[pos] += c;
		}
	}

	for (int i = 0; i < m_iRows * m_iColumns; i++)
	{
		for (int tileType = 0; tileType < m_iSizeOfTileTypeData; tileType++)
		{
			if (word[i] == std::to_string(tileType) || StringHelper::StringEqualsIgnoreCase(word[i], m_sTileTypeData[tileType].name))
			{
#if _DEBUG
				m_TileMapPreviewImageButtonColor[i] = m_sTileTypeData[tileType].color;
#endif
				if (m_tileMapLayer == TileMapLayer::Background)
				{
					m_tileMapBackground->UpdateTile(i, tileType);
				}
				else if (m_tileMapLayer == TileMapLayer::Foreground)
				{
					m_tileMapForeground->UpdateTile(i, tileType);
				}
			}
			else if (word[i] == "")
			{
#if _DEBUG
				m_TileMapPreviewImageButtonColor[i] = m_sTileTypeData[0].color;
#endif
				if (m_tileMapLayer == TileMapLayer::Background)
				{
					m_tileMapBackground->UpdateTile(i, 0);
				}
				else if (m_tileMapLayer == TileMapLayer::Foreground)
				{
					m_tileMapForeground->UpdateTile(i, 0);
				}
			}
		}
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
		if (OpenFileExplorer())
		{
			if (SaveWriteFile())
			{
				m_sSelectedFile += " Save Successful";
			}
			else
			{
				m_sSelectedFile = "Write File Failed";
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
	static char saveFileName[128] = "";
#if _DEBUG
	m_bSaveNewButton = ImGui::Button("Save To New File");
	ImGui::InputTextWithHint("##TileMapSaveFile", "New Save File Name", saveFileName, IM_ARRAYSIZE(saveFileName));
#endif

	if (m_bSaveNewButton)
	{
		if (OpenFileExplorer())
		{
			const size_t slash = m_sFilePath.find_last_of("/\\");
			m_sFilePath = m_sFilePath.substr(0, slash) + "\\" + saveFileName + ".txt";
			if (SaveWriteFile())
			{
				m_sSelectedFile = saveFileName;
				m_sSelectedFile += ".txt";
				m_sSelectedFile += " Save Successful";
			}
			else
			{
				m_sSelectedFile = "Can't save both";
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
	if (m_tileMapLayer == TileMapLayer::Both)
	{
		return false;
	}

	std::ofstream myFile(m_sFilePath);
	std::string line;
	std::string word;

	if (myFile.is_open())
	{
		for (int i = 0; i < m_iRows * m_iColumns; i++)
		{

			if (m_tileMapLayer == TileMapLayer::Background)
			{
				word = m_sTileTypeData[m_tileMapBackground->GetTileType(i)].name;
			}
			else if (m_tileMapLayer == TileMapLayer::Foreground)
			{
				word = m_sTileTypeData[m_tileMapForeground->GetTileType(i)].name;
			}			

			if (i + 1 != m_iRows * m_iColumns)
			{
				line += word + ",";
			}
			else
			{
				line += word;
			}

			if ((i + 1) % m_iColumns == 0)
			{
				myFile << line << "\n";
				line = "";
			}
		}
		myFile.close();

		return true;
	}
	else
	{
		return false;
	}
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
