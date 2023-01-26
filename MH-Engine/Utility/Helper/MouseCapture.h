#pragma once
#ifndef MOUSE_CAPTURE_H
#define MOUSE_CAPTURE_H

#include "EventSystem.h"
#include "Vector2f.h"

#if _DEBUG
#include <imgui/imgui.h>
#endif

struct MouseCaptureData
{
	Vector2f vMousePos;
	Vector2f vEditorPos;
	Vector2f vEditorSize;
	Vector2f vSceneSize;

	void DataOverlay()
	{
		ImGui::Text("MouseCapture");
		ImGui::Text("MousePos: %.2f, %.2f", vMousePos.x, vMousePos.y);
		ImGui::Text("EditorPos: %.2f, %.2f", vEditorPos.x, vEditorPos.y);
		ImGui::Text("EditorSize: %.2f, %.2f", vEditorSize.x, vEditorSize.y);
		ImGui::Text("SceneSize: %.2f, %.2f", vSceneSize.x, vSceneSize.y);
	}
};

class MouseCapture 
{
public:
	MouseCapture() {};
	~MouseCapture() {};

	static Vector2f GetGamePos(ImVec2 ivMousetPos, Vector2f vEditorPos, ImVec2 ivEditorSize, ImVec2 ivSceneSize);
	static bool IsInScene(Vector2f vMousePos, Vector2f vScenePos, Vector2f vSceneSize);
	
	static Vector2f EditorPosToGamePos(Vector2f vInputPos, Vector2f vEditorPos, Vector2f vEditorSize, Vector2f vGameSize);

	static MouseCaptureData GetOverlayData() { return m_data; }
	
private:
	static MouseCaptureData m_data;

};

#endif // !MOUSE_CAPTURE_H
