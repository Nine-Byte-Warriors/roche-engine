#pragma once
#ifndef STRINGHELPER_H
#define STRINGHELPER_H

/// <summary>
/// Primarily used to convert between narrow and wide strings.
/// </summary>
class StringHelper
{
public:
	static std::wstring StringToWide( const std::string& narrow ) noexcept;
	static std::string StringToNarrow( const std::wstring& wide ) noexcept;
	static std::string GetDirectoryFromPath( const std::string& filePath );
	static std::string GetFileExtension( const std::string& fileName );
	static bool StringEqualsIgnoreCase( const std::string& str1, const std::string& str2 );
};

#endif