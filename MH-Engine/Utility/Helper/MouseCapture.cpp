#include "stdafx.h"
#include "MouseCapture.h"

MouseCaptureData MouseCapture::m_data = 
{
	new Vector2f(0.0f, 0.0f), 
	new Vector2f(0.0f, 0.0f), 
	new Vector2f(0.0f, 0.0f), 
	new Vector2f(0.0f, 0.0f) 
};

Vector2f MouseCapture::GetGamePos(ImVec2 ivMousetPos, Vector2f vEditorPos, ImVec2 ivEditorSize, ImVec2 ivSceneSize)
{
	Vector2f vMousePos = { ivMousetPos.x, ivMousetPos.y };
	//Vector2f vEditorPos = { vEditorPos.x, vEditorPos.y };
	Vector2f vEditorSize = { ivEditorSize.x, ivEditorSize.y };
	Vector2f vSceneSize = { ivSceneSize.x, ivSceneSize.y };
	
	m_data.vMousePos = &vMousePos;
	m_data.vEditorPos = &vEditorPos;
	m_data.vEditorSize = &vEditorSize;	
	m_data.vSceneSize = &vSceneSize;
	
	if (MouseCapture::IsInScene(vMousePos, vEditorPos, vSceneSize))
		return EditorPosToGamePos(vMousePos, vEditorPos, vEditorSize, vSceneSize);
	
	return { -1.0f, -1.0f };
}

bool MouseCapture::IsInScene(Vector2f vMousePos, Vector2f vEditorPos, Vector2f vSceneSize)
{
	return (vMousePos.x > vEditorPos.x && vMousePos.x < (vEditorPos.x + vSceneSize.x) &&
		vMousePos.y > vEditorPos.y && vMousePos.y < (vEditorPos.y + vSceneSize.y))
		? true : false;
}

Vector2f MouseCapture::EditorPosToGamePos(Vector2f vInputPos, Vector2f vEditorPos, Vector2f vEditorSize, Vector2f vGameSize)
{
	return Vector2f
	(
		(vInputPos.x - vEditorPos.x) * (vGameSize.x / vEditorSize.x),
		(vInputPos.y - vEditorPos.y) * (vGameSize.y / vEditorSize.y)
	);
}
