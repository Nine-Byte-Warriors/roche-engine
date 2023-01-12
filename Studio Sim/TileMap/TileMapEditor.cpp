#include "stdafx.h"
#include "TileMapEditor.h"

TileMapEditor::TileMapEditor()
{

	m_sTileTypeData = tileMap.GetTileTypeData();
	m_iSizeOfTileTypeData = m_sTileTypeData.size();

	for (int i = 0; i < COLUMNS * ROWS; i++)
	{
		m_TileMapPreviewImageButtonColor[i] = m_sTileTypeData[0].color;
	}

	m_iCurrentSelectedTileType = 0;
	m_sCurrentSelectedTileType = "Current Tile Type: ";
	m_sCurrentSelectedTileType += m_sTileTypeData[0].name;

	m_bDrawOnce = false;
	m_bDrawContinuous = false;
}

void TileMapEditor::SpawnControlWindow()
{
	if (ImGui::Begin("Tile Map Editor", FALSE, ImGuiWindowFlags_AlwaysAutoResize))
	{
		Load();
		SaveToExistingFile();
		SaveToNewFile();

		DrawButton();

		TileMapSelectionButtons();
		TileMapSelectedText();
		TileMapGridPreview();
	}

	ImGui::End();
}

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

std::string TileMapEditor::GetTileTypeName(int pos)
{
	return m_sTileTypeData[tileMap.GetTileType(pos)].name;
}

void TileMapEditor::DrawButton()
{
	ImGui::Text("");
	m_bDrawButton = ImGui::Button("Draw Once");
	if (m_bDrawButton)
	{
		m_bDrawOnce = true;
	}
	ImGui::Checkbox("Draw Continuously", &m_bDrawContinuous);
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
	std::string word[COLUMNS * ROWS];
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

	for (int i = 0; i < COLUMNS * ROWS; i++)
	{
		for (int j = 0; j < m_iSizeOfTileTypeData; j++)
		{
			if (word[i] == std::to_string(j) || StringHelper::StringEqualsIgnoreCase(word[i], m_sTileTypeData[j].name))
			{
				m_TileMapPreviewImageButtonColor[i] = m_sTileTypeData[j].color;
				tileMap.UpdateTile(i, j);
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
	ImGui::InputTextWithHint("TileMapSaveFile", "New Save File Name", saveFileName, IM_ARRAYSIZE(saveFileName));

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
		for (int i = 0; i < COLUMNS * ROWS; i++)
		{
			std::string word = m_sTileTypeData[tileMap.GetTileType(i)].name;

			if (i + 1 != COLUMNS * ROWS)
			{
				line += word + ",";
			}
			else
			{
				line += word;
			}

			if ((i + 1) % COLUMNS == 0)
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

	for (int i = 0; i < m_iSizeOfTileTypeData; i++)
	{
		m_sTileTypeData[i].button =
			ImGui::ImageButtonNoTexture(m_sTileTypeData[i].name.c_str(), m_vImageButtonSize, m_vImageButtonFrame0, m_vImageButtonFrame1, m_iImageButtonPadding, m_sTileTypeData[i].color);

		ImGui::SameLine();
		ImGui::Text(m_sTileTypeData[i].name.c_str());
		if ((i + 1) % (COLUMNS / 2) != 0)
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
}

void TileMapEditor::TileMapSelectedText()
{
	ImGui::Text("");

	for (int i = 0; i < m_iSizeOfTileTypeData; i++)
	{
		if (m_iCurrentSelectedTileType == i)
		{
			m_sCurrentSelectedTileType = "Current Tile Type: ";
			m_sCurrentSelectedTileType += m_sTileTypeData[i].name;
		}
	}

	ImGui::Text(m_sCurrentSelectedTileType.c_str());
}

void TileMapEditor::TileMapGridPreview()
{
	ImGui::Text("");

	for (int i = 0; i < COLUMNS * ROWS; i++)
	{
		std::string gridLable = "Grid" + std::to_string(i);
		m_bTileMapPreviewImageButton[i] = ImGui::ImageButtonNoTexture(gridLable.c_str(), m_vImageButtonSize, m_vImageButtonFrame0, m_vImageButtonFrame1, m_iImageButtonPadding, m_TileMapPreviewImageButtonColor[i]);
		if ((i % COLUMNS) - (COLUMNS - 1) != 0)
		{
			ImGui::SameLine();
		}
	}

	UpdateTileMapGridPreview();
}

void TileMapEditor::UpdateTileMapGridPreview()
{
	for (int i = 0; i < COLUMNS * ROWS; i++)
	{
		if (m_bTileMapPreviewImageButton[i])
		{
			for (int j = 0; j < m_iSizeOfTileTypeData; j++)
			{
				if (m_iCurrentSelectedTileType == tileMap.GetTileTypeData()[j].type)
				{
					m_TileMapPreviewImageButtonColor[i] = m_sTileTypeData[j].color;
					tileMap.UpdateTile(i, tileMap.GetTileTypeData()[j].type);
				}
			}
		}
	}
}