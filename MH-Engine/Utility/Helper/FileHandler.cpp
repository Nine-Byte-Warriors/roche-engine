#include "stdafx.h"
#include "FileHandler.h"

FileHandler::FileHandler(std::shared_ptr<FileHandler::FileObject>& foFile)
{
	m_fileObject = foFile != nullptr
		? foFile
		: CreateFileObject();

	m_pOpenDialog = nullptr;
	m_pSaveDialog = nullptr;
}

std::shared_ptr<FileHandler> FileHandler::UseOpenDialog()
{
	m_hResult = CoCreateInstance(
		CLSID_FileOpenDialog,
		NULL,
		CLSCTX_ALL,
		IID_IFileOpenDialog,
		reinterpret_cast<void**>(&m_pOpenDialog)
	);

	if (FAILED(m_hResult)) {
		CleanUp();
		return nullptr;
	}

	return shared_from_this();
}

std::shared_ptr<FileHandler> FileHandler::UseSaveDialog()
{
	m_hResult = CoCreateInstance(
		CLSID_FileSaveDialog,
		NULL,
		CLSCTX_ALL,
		IID_IFileSaveDialog,
		reinterpret_cast<void**>(&m_pSaveDialog)
	);

	if (FAILED(m_hResult)) {
		CleanUp();
		return nullptr;
	}

	return shared_from_this();
}

std::shared_ptr<FileHandler> FileHandler::ShowDialog()
{
	if (m_pSaveDialog == nullptr && m_pOpenDialog == nullptr)
	{
		m_fileObject->m_sErrorMsg
			.append("\nNo Dialog selected. Use 'UseSaveDialog' or 'UseOpenDialog'.");
		return nullptr;
	}

	//  SHOW FILE DIALOG WINDOW
	m_hResult = m_pOpenDialog == nullptr
		? m_pSaveDialog->Show(NULL)
		: m_pOpenDialog->Show(NULL);

	if (FAILED(m_hResult)) {
		CleanUp();
		return nullptr;
	}

	return shared_from_this();
}

std::shared_ptr<FileHandler::FileObject> FileHandler::StoreDialogResult()
{
	if (m_pSaveDialog == nullptr && m_pOpenDialog == nullptr)
	{
		m_fileObject->m_sErrorMsg
			.append("\nNo Dialog selected. Use 'UseSaveDialog' or 'UseOpenDialog'.");
		return nullptr;
	}

	//  RETRIEVE FILE NAME FROM THE SELECTED ITEM
	IShellItem* pShellFile;
	m_hResult = m_pOpenDialog == nullptr
		? m_pSaveDialog->GetResult(&pShellFile)
		: m_pOpenDialog->GetResult(&pShellFile);

	if (FAILED(m_hResult)) {
		CleanUp();

		CoUninitialize();
		return nullptr;
	}

	//  STORE AND CONVERT THE FILE NAME
	PWSTR pwcPath;
	m_hResult = pShellFile->GetDisplayName(SIGDN_FILESYSPATH, &pwcPath);
	if (FAILED(m_hResult)) {
		pShellFile->Release();

		CleanUp();
		return nullptr;
	}

	SetPath(pwcPath);

	//  SUCCESS, CLEAN UP
	CoTaskMemFree(pwcPath);
	pShellFile->Release();
	CleanUp();

	return m_fileObject;
}

std::shared_ptr<FileHandler::FileObject> FileHandler::SetPath(const std::wstring wsPath)
{
	std::string sFullPath(wsPath.begin(), wsPath.end());

	const size_t lSlash = sFullPath.find_last_of("/\\");
	m_fileObject->m_sPath = sFullPath.substr(0, lSlash);

	std::string sFileName = sFullPath.substr(lSlash + 1);

	const size_t lExt = sFileName.find_last_of(".");
	m_fileObject->m_sFile = sFileName.substr(0, lExt);

	m_fileObject->m_sExt = lExt != std::string::npos
		? sFileName.substr(lExt + 1)
		: "";

	return m_fileObject;
}

void FileHandler::CleanUp()
{
	if (m_pOpenDialog != nullptr)
		m_pOpenDialog->Release();

	if (m_pSaveDialog != nullptr)
		m_pSaveDialog->Release();

	m_pSaveDialog = nullptr;
	m_pOpenDialog = nullptr;

	CoUninitialize();
}
