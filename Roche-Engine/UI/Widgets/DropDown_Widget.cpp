#include "stdafx.h"
#include "DropDown_Widget.h"

DropDown_Widget::DropDown_Widget()
{
	m_spriteBack = std::make_shared<Sprite>();
    m_transformBack = std::make_shared<Transform>( m_spriteBack );
}

DropDown_Widget::DropDown_Widget( const std::vector<std::string>& ddList, std::vector<std::string> backCol, std::vector<std::string> buttonImg, XMVECTORF32 textColour, std::string currData, MouseData& mData )
{
	m_spriteBack = std::make_shared<Sprite>();
    m_transformBack = std::make_shared<Transform>( m_spriteBack );

	Resolve( ddList, backCol, buttonImg, textColour, currData, mData );
}

DropDown_Widget::~DropDown_Widget() { }

void DropDown_Widget::Initialize( ID3D11Device* device, ID3D11DeviceContext* context, ConstantBuffer<Matrices>& mat )
{
	m_iFlag = m_iFlagMax;

	m_spriteBack->Initialize( device, context, "", mat, m_vSize.x, m_vSize.y );
    m_transformBack->SetPositionInit( m_vPosition.x, m_vPosition.y );
	m_transformBack->SetScaleInit( m_vSize.x, m_vSize.y );

	m_wButtonDrop.Initialize( device, context, mat );
	for ( unsigned int i = 0; i < ARRAYSIZE( m_wListButtons ); i++ )
		m_wListButtons[i].Initialize( device, context, mat );
}

void DropDown_Widget::Update( const float dt )
{
	m_spriteBack->Update( dt );
	m_transformBack->Update();

	m_wButtonDrop.Update( dt );

	if ( m_vListData.size() <= 0 ) return;
	for ( unsigned int i = 0; i < m_vListData.size(); i++ )
		m_wListButtons[i].Update( dt );
}

void DropDown_Widget::Draw( ID3D11Device* device, ID3D11DeviceContext* context, XMMATRIX worldOrtho, TextRenderer* textRenderer, VertexShader& vert, PixelShader& pix )
{
	// Background
	m_spriteBack->UpdateTex( device, "Resources\\Textures\\UI\\Drop-Down\\DropDownMain.png" );
	m_spriteBack->UpdateBuffers( context );
	m_spriteBack->Draw( m_transformBack->GetWorldMatrix(), worldOrtho );

	// Drop-down button
	m_wButtonDrop.Draw( device, context, worldOrtho, textRenderer );
	if ( m_vListData.size() <= 0 ) return;

	Shaders::BindShaders( context, vert, pix );
	if ( m_eDropState == DropState::Down )
	{
		for ( unsigned int i = 0; i < m_vListData.size(); i++ )
		{
			m_wListButtons[i].Draw( device, context, worldOrtho, textRenderer );
			Shaders::BindShaders( context, vert, pix );
		}
	}
	
	XMVECTOR textsize = textRenderer->GetSpriteFont()->MeasureString( m_vListData[m_iSelected].c_str() );
	XMFLOAT2 textpos =
	{
		m_vPosition.x + ( m_vSize.x / 2.0f ) - ( XMVectorGetX( textsize ) * textRenderer->GetScale().x ) / 2.0f,
		m_vPosition.y + ( m_vSize.y / 2.0f ) - ( XMVectorGetY( textsize ) * textRenderer->GetScale().y ) / 2.0f
	};
	textRenderer->RenderString( m_vListData[m_iSelected], textpos, m_vTextColour, false );
}

void DropDown_Widget::Resolve( const std::vector<std::string>& ddList, std::vector<std::string> backCol, std::vector<std::string> buttonImg, XMVECTORF32 textColour, std::string currData, MouseData& mData )
{
	m_vListData = ddList;
	m_vTextColour = textColour;

    m_transformBack->SetPosition( m_vPosition.x, m_vPosition.y );
    m_transformBack->SetScale( m_vSize.x, m_vSize.y );
    m_spriteBack->SetWidth( m_vSize.x );
    m_spriteBack->SetHeight( m_vSize.y );

	m_wButtonDrop.Resolve( "", textColour, buttonImg, mData );
	m_wButtonDrop.SetPosition( { m_vPosition.x + m_vSize.x, m_vPosition.y } );
	m_wButtonDrop.SetSize( { m_vSize.y, m_vSize.y } );
	for ( unsigned int i = 0; i < m_vListData.size(); i++ )
		if ( currData == m_vListData[i] )
			m_iSelected = i;

	// List buttons
	switch ( m_eDropState )
	{
	case DropState::Down:
	{
		float PosY = m_vPosition.y + m_vSize.y;
		for ( unsigned int i = 0; i < ddList.size(); i++ )
		{
			m_wListButtons[i].SetSize( { m_vSize.x,m_vSize.y } );
			m_wListButtons[i].SetPosition( { m_vPosition.x, PosY } );
			if ( m_wListButtons[i].Resolve( m_vListData[i], textColour, backCol, mData ) )
			{
				m_eDropState = DropState::Up;
				m_iSelected = i;
				m_iFlag = 0;
			}
			PosY += m_vSize.y;
		}

		if ( m_wButtonDrop.GetIsPressed() && m_iFlag == m_iFlagMax )
		{
			m_eDropState = DropState::Up;
			m_iFlag = 0;
		}
		else if( m_iFlag < m_iFlagMax )
		{
			m_iFlag++;
		}
	}
	break;
	case DropState::Up:
	{
		if ( m_wButtonDrop.GetIsPressed() && m_iFlag == m_iFlagMax )
		{
			m_eDropState = DropState::Down;
			m_iFlag = 0;
		}
		else if ( m_iFlag < m_iFlagMax )
		{
			m_iFlag++;
		}
	}
	default:
		break;
	}

	m_sDataSelected = m_vListData[m_iSelected];
}