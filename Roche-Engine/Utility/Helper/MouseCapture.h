#pragma once
#ifndef MOUSE_CAPTURE_H
#define MOUSE_CAPTURE_H

#if _DEBUG
class MouseCapture : public Listener
{
public:
	static Vector2f GetGamePos(ImVec2 ivMousetPos, Vector2f vEditorPos, ImVec2 ivEditorSize, ImVec2 ivSceneSize);
	static Vector2f EditorPosToGamePos(Vector2f vInputPos, Vector2f vEditorPos, Vector2f vEditorSize, Vector2f vGameSize);
};
#endif

#endif // !MOUSE_CAPTURE_H
