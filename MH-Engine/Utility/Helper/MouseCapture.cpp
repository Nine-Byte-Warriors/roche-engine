#include "stdafx.h"
#include "MouseCapture.h"

#if _DEBUG
Vector2f MouseCapture::GetGamePos(ImVec2 ivMousePos, Vector2f vEditorPos, ImVec2 ivEditorSize, ImVec2 ivSceneSize)
{
	Vector2f vMousePos = { ivMousePos.x, ivMousePos.y };
	Vector2f vEditorSize = { ivEditorSize.x, ivEditorSize.y };
	Vector2f vSceneSize = { ivSceneSize.x, ivSceneSize.y };

	return EditorPosToGamePos(vMousePos, vEditorPos, vEditorSize, vSceneSize);
}

Vector2f MouseCapture::EditorPosToGamePos(Vector2f vInputPos, Vector2f vEditorPos, Vector2f vEditorSize, Vector2f vGameSize)
{
	return Vector2f
	(
		(vInputPos.x - vEditorPos.x) * (vGameSize.x / vEditorSize.x),
		(vInputPos.y - vEditorPos.y) * (vGameSize.y / vEditorSize.y)
	);
}
#endif