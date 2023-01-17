#pragma once
#ifndef FILELOADING_H
#define FILELOADING_H

/// <summary>
/// Primarily used to help with reading from and writing to files.
/// </summary>
class FileLoading
{
public:
	static bool OpenFileExplorer( std::string& selectedFile, std::string& filePath );
};

#endif