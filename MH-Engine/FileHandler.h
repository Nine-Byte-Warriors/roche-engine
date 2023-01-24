#pragma once
#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <ShObjIdl.h>
#include <string.h>

class FileHandler : public std::enable_shared_from_this<FileHandler>
{
public:
	struct FileObject
	{
	public:
		std::string m_sPath;
		std::string m_sFile;
		std::string m_sExt;

		std::string m_sErrorMsg;

		std::string GetFullPath()
		{
			if (!HasPath()) return "";

			if (m_sExt.empty()) m_sExt = ".json";
			return m_sPath + "/\\" + m_sFile + "." + m_sExt;
		}

		bool HasPath()
		{
			if (m_sPath.empty()) return false;
			if (m_sFile.empty()) return false;
			return true;
		}
	};

	FileHandler(std::shared_ptr<FileHandler::FileObject>& foFile)
	{
		m_fileObject = foFile != nullptr
			? foFile
			: CreateFileObject();

		m_pOpenDialog = nullptr;
		m_pSaveDialog = nullptr;
	}
	~FileHandler() {};

	static bool OpenFileDialog(std::string& selectedFile, std::string& filePath);
	static bool SaveFileDialog(std::string& selectedFile, std::string& filePath);

	static std::shared_ptr<FileHandler::FileObject> CreateFileObject()
	{
		std::shared_ptr<FileHandler::FileObject> pFileObject = std::make_shared<FileHandler::FileObject>();
		pFileObject->m_sPath = "";
		pFileObject->m_sFile = "";
		pFileObject->m_sExt = "";
		pFileObject->m_sErrorMsg = "";
		return pFileObject;
	}

	static std::shared_ptr<FileHandler> FileDialog(std::shared_ptr<FileHandler::FileObject> foFile)
		{ return std::make_shared<FileHandler>(foFile); }
	
	std::shared_ptr<FileHandler> UseSaveDialog()
	{
		//m_fileObject = CreateFileObject();
		
		//m_hResult = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
		
		//m_pSaveDialog = std::make_unique<IFileSaveDialog>();

		m_hResult = CoCreateInstance(
			CLSID_FileSaveDialog,
			NULL,
			CLSCTX_ALL,
			IID_IFileSaveDialog,
			reinterpret_cast<void**>(&m_pSaveDialog)
		);

		if (FAILED(m_hResult)) {
			CoUninitialize();
			return nullptr;
		}

		return shared_from_this();
	}

	std::shared_ptr<FileHandler> ShowDialog()
	{
		if (m_pSaveDialog == nullptr && m_pOpenDialog == nullptr)
		{
			m_fileObject->m_sErrorMsg = "No Dialog selected. Use 'UseSaveDialog' or 'UseOpenDialog'.";
			m_hResult = false;
			return nullptr;
		}

		//  SHOW FILE DIALOG WINDOW
		m_hResult = m_pOpenDialog == nullptr
			? m_pSaveDialog->Show(NULL)
			: m_pOpenDialog->Show(NULL);

		if (FAILED(m_hResult)) {
			if (m_pOpenDialog != nullptr)
				m_pSaveDialog->Release();
			else
				m_pOpenDialog->Release();

			CoUninitialize();
			return nullptr;
		}

		return shared_from_this();
	}

	std::shared_ptr<FileObject> StoreResult()
	{
		//  RETRIEVE FILE NAME FROM THE SELECTED ITEM
		IShellItem* pShellFile;
		m_hResult = m_pOpenDialog == nullptr
			? m_pSaveDialog->GetResult(&pShellFile)
			: m_pOpenDialog->GetResult(&pShellFile);

		if (FAILED(m_hResult)) {
			if (m_pOpenDialog != nullptr)
				m_pSaveDialog->Release();
			else
				m_pOpenDialog->Release();

			CoUninitialize();
			return nullptr;
		}

		//  STORE AND CONVERT THE FILE NAME
		PWSTR pwcPath;
		m_hResult = pShellFile->GetDisplayName(SIGDN_FILESYSPATH, &pwcPath);
		if (FAILED(m_hResult)) {
			pShellFile->Release();

			if (m_pOpenDialog != nullptr)
				m_pSaveDialog->Release();
			else
				m_pOpenDialog->Release();

			CoUninitialize();
			return nullptr;
		}

		SetPath(pwcPath);

		//  SUCCESS, CLEAN UP
		CoTaskMemFree(pwcPath);
		pShellFile->Release();
		if (m_pOpenDialog != nullptr)
			m_pOpenDialog->Release();
		else
			m_pSaveDialog->Release();
		CoUninitialize();

		return m_fileObject;
	}

private:
	std::shared_ptr<FileObject> SetPath(const std::wstring wsPath)
	{
		std::string sFullPath(wsPath.begin(), wsPath.end());

		const size_t lSlash = sFullPath.find_last_of("/\\");
		m_fileObject->m_sPath = sFullPath.substr(0, lSlash);

		m_fileObject->m_sFile = sFullPath.substr(lSlash + 1);

		const size_t lExt = sFullPath.find_last_of(".");
		m_fileObject->m_sExt = lExt != std::string::npos
			? sFullPath.substr(lExt)
			: "";

		return m_fileObject;
	}

	std::shared_ptr<FileHandler::FileObject> m_fileObject;
	
	IFileSaveDialog* m_pSaveDialog;
	IFileOpenDialog* m_pOpenDialog;

	HRESULT m_hResult;

};

#endif // !FILE_HANDLER_H