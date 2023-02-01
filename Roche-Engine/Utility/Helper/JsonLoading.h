#pragma once
#ifndef JSONLOADING_H
#define JSONLOADING_H

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class JsonLoading
{
public:
	template <typename t>
	static void LoadJson(std::vector<t>& structData, std::string inputfile);
	template <typename t>
	static void SaveJson(std::vector<t> structData, std::string inputfile);
	template <typename t>
	static void LoadJson(t& structData, std::string inputfile);
	template <typename t>
	static void SaveJson(t structData, std::string inputfile);
};

//Dont put these in the cpp, it will brake
template<typename t>
static void JsonLoading::LoadJson(std::vector<t>& structData, std::string inputfile)
{
	try
	{
		std::ifstream jsonFile(inputfile);
		json jsonData = json::parse(jsonFile);
		structData = jsonData;
	}
	catch (const std::exception) {}
}

template<typename t>
static void JsonLoading::SaveJson(std::vector<t> structData, std::string inputfile)
{
	try
	{
		json jsonData = structData;
		std::ofstream o(inputfile);
		o << std::setw(4) << jsonData << std::endl;
	}
	catch (const std::exception) {}
}

template<typename t>
static void JsonLoading::LoadJson(t& structData, std::string inputfile)
{
	try
	{
		std::ifstream jsonFile(inputfile);
		json jsonData = json::parse(jsonFile);
		structData = jsonData;
	}
	catch (const std::exception) {}
}

template<typename t>
static void JsonLoading::SaveJson(t structData, std::string inputfile)
{
	try
	{
		json jsonData = structData;
		std::ofstream o(inputfile);
		o << std::setw(4) << jsonData << std::endl;
	}
	catch (const std::exception) {}
}
#endif
