#pragma once
#ifndef WINDOWCONTAINER_H
#define WINDOWCONTAINER_H

#include "Mouse.h"
#include "Keyboard.h"
#include "Vector2f.h"
#include "EventSystem.h"
#include "RenderWindow.h"

class WindowContainer : public Listener
{
public:
	WindowContainer();
	LRESULT CALLBACK WindowProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void EnableCursor() noexcept;
	void DisableCursor() noexcept;
	inline bool IsCursorEnabled() const noexcept { return m_bCursorEnabled; }

	void AddToEvent() noexcept;
	void HandleEvent( Event* event ) override;

protected:
	void ConfineCursor() noexcept;
	void FreeCursor() noexcept;
	void ShowCursor() noexcept;
	void HideCursor() noexcept;
	void EnableImGuiMouse() noexcept;
	void DisableImGuiMouse() noexcept;

	Vector2f m_vImguiPos = { 0.0f, 0.0f };
	bool m_bCursorEnabled = true;
	XMFLOAT2 m_windowSize;

	RenderWindow m_renderWindow;
	Keyboard m_keyboard;
	Mouse m_mouse;
};

#endif