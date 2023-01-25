#pragma once
#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <ShObjIdl.h>

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

		FileObject(std::string sFileName)
		{
			m_sPath = "";
			m_sFile = sFileName;
			m_sExt = "";
			m_sErrorMsg = "";
		}

		bool HasPath() { return m_sPath.empty() ? false : true; }

		std::string GetFullPath()
		{
			if (!HasPath()) return "";

			std::string sFullPath(m_sPath);
			return sFullPath.append("\\" + GetFilePath());
		}

		std::string GetFilePath()
		{
			std::string sFilePath("");
			if (!m_sFile.empty()) sFilePath.append(m_sFile);
			if (!m_sExt.empty()) sFilePath.append("." + m_sExt);
			return sFilePath;
		}

		std::string GetJsonPath()
		{
			if (!HasPath()) return "";

			if (m_sExt.empty()) m_sExt = "json";
			return m_sPath + "\\" + m_sFile + "." + m_sExt;
		}
	};

	FileHandler(std::shared_ptr<FileHandler::FileObject>& foFile);
	~FileHandler() {};

	inline static std::shared_ptr<FileHandler::FileObject> CreateFileObject(std::string sFileName = "")
		{ return std::make_shared<FileHandler::FileObject>(sFileName); }

	inline static std::shared_ptr<FileHandler> FileDialog(std::shared_ptr<FileHandler::FileObject> foFile)
		{ return std::make_shared<FileHandler>(foFile); }
	
	std::shared_ptr<FileHandler> UseOpenDialog();
	std::shared_ptr<FileHandler> UseSaveDialog();
	std::shared_ptr<FileHandler> ShowDialog();
	std::shared_ptr<FileObject> StoreDialogResult();

private:
	std::shared_ptr<FileObject> SetPath(const std::wstring wsPath);
	void CleanUp();
	
	std::shared_ptr<FileHandler::FileObject> m_fileObject;
	
	IFileSaveDialog* m_pSaveDialog;
	IFileOpenDialog* m_pOpenDialog;

	HRESULT m_hResult;
};

#endif // !FILE_HANDLER_H