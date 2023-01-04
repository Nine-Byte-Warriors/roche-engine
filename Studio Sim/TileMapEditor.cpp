#include "stdafx.h"
#include "TileMapEditor.h"
#include <imgui/imgui.h>
#include <TileMap.h>

//https://github.com/dibidabidab/game-without-a-name-and-unicorns/blob/master/source/rendering/level/LevelEditor.cpp
void TileMapEditor::SpawnControlWindow()
{
	if (ImGui::Begin("Tile Map Editor", FALSE, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
	{
		static std::string map = "WALL, WALL, WALL, WALL, WALL\nWALL, DIRT, DIRT, DIRT, WALL\nWALL, DIRT, DIRT, DIRT, WALL\nWALL, DIRT, DIRT, DIRT, WALL\nWALL, WALL, WALL, WALL, WALL";
		std::string temp2 = "";

		ImGui::Text("Tile Map Editor");
		ImGui::Text("EMPTY");
		ImGui::Text("DIRT");
		if (ImGui::Button("WALL"))
		{
			map == temp2;
			ImGui::Text(map.c_str());
			ImGui::SetMouseCursor(117);
		}
		//ImGui::ImageButton("WALL");
		if (ImGui::ImageButton(nullptr, ImVec2(32, 32), ImVec2(0, 0), ImVec2(32.0f, 32.0f), 2, ImColor(0, 0, 0, 255)))
		{
			int temp = 12;
			int temp2 = 12;
		}
		ImGui::SameLine();
		ImGui::Text("EMPTY");
	
		ImGui::ImageButton(nullptr, ImVec2(32, 32), ImVec2(0, 0), ImVec2(32.0f, 32.0f), 2, ImColor(40, 40, 40, 255));
		ImGui::ImageButton(nullptr, ImVec2(32, 32), ImVec2(0, 0), ImVec2(32.0f, 32.0f), 2, ImColor(220, 40, 40, 255));
		//ImGui::SameLine();
		//ImGui::Button("WALL2");


		ImGui::Text(map.c_str());

		//for (int i = 0; i < 25; i++)
		//{
		//	map += "";
		//}
		


	}

	ImGui::End();
}
