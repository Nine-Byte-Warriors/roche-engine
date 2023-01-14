#pragma once
#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include "UI.h"
#include "EventSystem.h"

/// <summary>
/// Manager class for each of the UI components.
/// </summary>
class UIManager : public Listener
{
public:
	UIManager() { AddToEvent(); }
	~UIManager() { RemoveAllUI(); RemoveFromEvent(); }

	void Initialize( const Graphics& gfx, ConstantBuffer<Matrices>* mat );
	void Update( const float dt );
	void Draw( XMMATRIX worldOrtho );

	std::shared_ptr<UI> GetCustomUI( const std::string& name );
	void AddUI( std::shared_ptr <UI> newUI, const std::string& name );
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

	XMFLOAT4X4 WorldOrthMatrix;
	std::unordered_map<std::string, std::shared_ptr<UI>> m_mUiList;
	std::vector<std::string> m_vUiToDraw;
	XMFLOAT2 m_vWindowSize;
};

#endif