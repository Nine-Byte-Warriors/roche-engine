#pragma once
#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include "UIScreen.h"
#include "EventSystem.h"

/// <summary>
/// Manager class for each of the UI components.
/// </summary>
class UIManager : public Listener
{
public:
	UIManager() { AddToEvent(); }
	~UIManager() { RemoveAllUI(); RemoveFromEvent(); }

	void Initialize( const Graphics& gfx, ConstantBuffer<Matrices>* mat, const std::vector<std::vector<Widget>>& widgets, Health& health );
	void Update( const float dt, const std::vector<std::vector<Widget>>& widgets );
	void Draw( VertexShader vtx, PixelShader pix, XMMATRIX worldOrtho, TextRenderer* textRenderer );

	std::shared_ptr<UIScreen> GetCustomUI( const std::string& name );
	void AddUI( std::shared_ptr <UIScreen> newUI, const std::string& name );
	void RemoveUI( const std::string& name );
	void RemoveAllUI();

	// Hiding/Showing UI
	void HideAllUI();
	void ShowAllUI();
	void ShowUI( const std::string& name );
	void HideUI( const std::string& name );

	void HandleEvent( Event* event ) override;
private:
	void AddToEvent();
	void RemoveFromEvent();

	std::unordered_map<std::string, std::shared_ptr<UIScreen>> m_mUiList;
	std::vector<std::string> m_vUiToDraw;
	XMFLOAT2 m_vWindowSize;
};

#endif