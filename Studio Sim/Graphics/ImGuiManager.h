#pragma once
#ifndef IMGUIMANAGER_H
#define IMGUIMANAGER_H

#if _DEBUG
#include "EventSystem.h"

/// <summary>
/// The main manager class for setting up ImGui components.
/// Contains a function to control the main graphical components of a scene from Graphics.h
/// </summary>
class ImGuiManager : public Listener
{
public:
	ImGuiManager();
	~ImGuiManager();
	void Initialize( HWND hWnd, ID3D11Device* device, ID3D11DeviceContext* context ) const noexcept;
	void BeginRender() const noexcept;
	void EndRender() const noexcept;
	void SpawnInstructionWindow() const noexcept;

	void AddToEvent() noexcept;
	void HandleEvent( Event* event ) override;
private:
	void SetBlackGoldStyle();
	XMFLOAT2 m_vWindowSize;
};
#endif

#endif