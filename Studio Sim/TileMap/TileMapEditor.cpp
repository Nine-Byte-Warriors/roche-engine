#include "stdafx.h"
#include "TileMapEditor.h"

TileMapEditor::TileMapEditor()
{
	m_iCurrentSelectedTileType = EMPTY;
	m_sCurrentSelectedTileType = "Current Tile Type: EMPTY";

	for (int i = 0; i < ROWS * COLUMNS; i++)
	{
		m_TileMapPreviewImageButtonColor[i] = ImColor(0, 0, 0, 255);
	}

	InitializeTileMapDataScruct();
}

void TileMapEditor::InitializeTileMapDataScruct()
{
	for (int i = 0; i < SIZEOFTILETYPE; i++)
	{
		m_sTileMapData[i].type = tileMap.GetTileTypeData()[i].type;
		m_sTileMapData[i].color = tileMap.GetTileTypeData()[i].color;
		m_sTileMapData[i].name = tileMap.GetTileTypeData()[i].name;
	}
}

void TileMapEditor::SpawnControlWindow()
{
	if (ImGui::Begin("Tile Map Editor", FALSE, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
	{
		Load();
		SaveToExistingFile();
		SaveToNewFile();

		TileMapSelectionButtons();
		TileMapSelectedText();
		TileMapGridPreview();
	}

	ImGui::End();
}

void TileMapEditor::Load()
{
	m_bLoadButton = ImGui::Button("Load");
	ImGui::Text(m_sSelectedFile.c_str());

	if (m_bLoadButton)
	{
		if (OpenFileExplorer())
		{
			if (LoadReadFile())
			{
				LoadProcessFile();
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

void TileMapEditor::LoadProcessFile()
{
	std::string word[ROWS * COLUMNS];
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

	for (int i = 0; i < ROWS * COLUMNS; i++)
	{
		for (int j = 0; j < SIZEOFTILETYPE; j++)
		{
			if (word[i] == std::to_string(j))
			{
				m_TileMapPreviewImageButtonColor[i] = m_sTileMapData[j].color;
			}
		}

	}
}

void TileMapEditor::SaveToExistingFile()
{
	ImGui::Text("");
	m_bSaveButton = ImGui::Button("Save To Existing File");

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
	m_bSaveNewButton = ImGui::Button("Save To New File");

	static char saveFileName[128] = "";
	ImGui::InputTextWithHint("", "New Save File Name", saveFileName, IM_ARRAYSIZE(saveFileName));

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
				m_sSelectedFile = "Write File Failed";
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
	std::ofstream myFile(m_sFilePath);
	std::string line;

	if (myFile.is_open())
	{
		for (int i = 0; i < ROWS * COLUMNS; i++)
		{
			if (i + 1 != ROWS * COLUMNS)
			{
				line += std::to_string(tileMap.GetTile(i)) + ",";
			}
			else
			{
				line += std::to_string(tileMap.GetTile(i));
			}

			if ((i + 1) % ROWS == 0)
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
	ImGui::Text("");

	for (int i = 0; i < SIZEOFTILETYPE; i++)
	{
		m_sTileMapData[i].button =
			ImGui::ImageButtonNoTexture(m_sTileMapData[i].name.c_str(), m_vImageButtonSize, m_vImageButtonFrame0, m_vImageButtonFrame1, m_iImageButtonPadding, m_sTileMapData[i].color);

		ImGui::SameLine();
		ImGui::Text(m_sTileMapData[i].name.c_str());
	}

	for (int i = 0; i < SIZEOFTILETYPE; i++)
	{
		if (m_sTileMapData[i].button)
		{
			m_iCurrentSelectedTileType = m_sTileMapData[i].type;
		}
	}
}

void TileMapEditor::TileMapSelectedText()
{
	ImGui::Text("");

	if (m_iCurrentSelectedTileType == EMPTY)
	{
		m_sCurrentSelectedTileType = "Current Tile Type: EMPTY";
	}
	else if (m_iCurrentSelectedTileType == DIRT)
	{
		m_sCurrentSelectedTileType = "Current Tile Type: DIRT";
	}
	else if (m_iCurrentSelectedTileType == WALL)
	{
		m_sCurrentSelectedTileType = "Current Tile Type: WALL";
	}

	ImGui::Text(m_sCurrentSelectedTileType.c_str());
}

void TileMapEditor::TileMapGridPreview()
{
	ImGui::Text("");

	for (int i = 0; i < ROWS * COLUMNS; i++)
	{
		std::string gridLable = "Grid" + std::to_string(i);
		m_bTileMapPreviewImageButton[i] = ImGui::ImageButtonNoTexture(gridLable.c_str(), m_vImageButtonSize, m_vImageButtonFrame0, m_vImageButtonFrame1, m_iImageButtonPadding, m_TileMapPreviewImageButtonColor[i]);
		if ((i % ROWS) - (ROWS - 1) != 0)
		{
			ImGui::SameLine();
		}
	}

	UpdateTileMapGridPreview();
}

void TileMapEditor::UpdateTileMapGridPreview()
{
	for (int i = 0; i < ROWS * COLUMNS; i++)
	{
		if (m_bTileMapPreviewImageButton[i])
		{
			for (int j = 0; j < SIZEOFTILETYPE; j++)
			{
				if (m_iCurrentSelectedTileType == tileMap.GetTileTypeData()[j].type)
				{
					m_TileMapPreviewImageButtonColor[i] = m_sTileMapData[j].color;
					tileMap.UpdateTile(i, tileMap.GetTileTypeData()[j].type);
				}
			}
		}
	}
}