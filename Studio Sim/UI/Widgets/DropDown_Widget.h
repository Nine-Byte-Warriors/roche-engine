#pragma once
#ifndef DROPDOWN_WIDGET_H
#define DROPDOWN_WIDGET_H

#include "Widget.h"
#include "Button_Widget.h"
#include "Shaders.h"

class DropDown_Widget : public Widget
{
public:
	enum class DropState { Down, Up };
	DropDown_Widget();
    DropDown_Widget( const std::vector<std::string>& ddList, XMFLOAT2 pos, XMFLOAT2 size, std::vector<std::string> backCol, std::vector<std::string> buttonImg, XMVECTORF32 textColour, std::string currData, MouseData mData );
	~DropDown_Widget();
   
	void Initialize( ID3D11Device* device, ID3D11DeviceContext* context, ConstantBuffer<Matrices>& mat );
	void Update( const float dt );
	void Draw( ID3D11Device* device, ID3D11DeviceContext* context, XMMATRIX worldOrtho, TextRenderer* textRenderer, VertexShader& vert, PixelShader& pix );
	void Resolve( const std::vector<std::string>& ddList, XMFLOAT2 pos, XMFLOAT2 size, std::vector<std::string> backCol, std::vector<std::string> buttonImg, XMVECTORF32 textColour, std::string currData, MouseData mData );

	inline int GetIntSelcted() const noexcept { return m_iSelected; }
    inline std::string GetSelected() const noexcept { return m_sDataSelected; }
	inline void SetCurrent( int selected ) noexcept { m_iSelected = selected; }
	inline bool GetIsDown() noexcept { if ( m_eDropState == DropState::Down ) { return true; } return false; }

private:
	int m_iFlag;
	int m_iFlagMax = 20;
	XMFLOAT2 m_vPosition, m_vSize;
    
	int m_iSelected = 0;
    std::string m_sDataSelected;
    std::vector<std::string> m_vListData;

	XMVECTORF32 m_vTextColour;
    std::shared_ptr<Sprite> m_spriteBack;
    std::shared_ptr<Transform> m_transformBack;

    Button_Widget m_wButtonDrop;
	Button_Widget m_wListButtons[2];
	DropState m_eDropState = DropState::Up;
};

#endif