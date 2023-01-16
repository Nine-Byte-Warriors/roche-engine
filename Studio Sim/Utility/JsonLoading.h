#pragma once
#ifndef JSONLOADING_H
#define JSONLOADING_H

#include <nlohmann/json.hpp>
using json = nlohmann::json;


class JsonLoading
{
public:
	template <typename jsonStructs>
	static void LoadJson(std::vector<jsonStructs>& structData, std::string inputfile);
	template <typename jsonStructs>
	static void SaveJson(std::vector<jsonStructs> structData, std::string inputfile);
};

//Dont put these in the cpp, it will brake
template<typename jsonStructs>
static void JsonLoading::LoadJson(std::vector<jsonStructs>& structData, std::string inputfile)
{
	std::ifstream jsonFile(inputfile);
	json jsonData = json::parse(jsonFile);
	structData = jsonData;
}

template<typename jsonStructs>
static void JsonLoading::SaveJson(std::vector<jsonStructs> structData, std::string inputfile)
{
	json jsonData = structData;
	std::ofstream o(inputfile);
	o << std::setw(4) << jsonData << std::endl;
}
#endif
