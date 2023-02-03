#include "stdafx.h"
#include "DataSlider_Widget.h"

DataSlider_Widget::DataSlider_Widget( XMFLOAT2 pos, XMFLOAT2 size )
{
	m_spriteBar = std::make_shared<Sprite>();
    m_spriteBar->SetWidthHeight( size.x, size.y );
    m_transformBar = std::make_shared<Transform>( m_spriteBar );
    m_transformBar->SetPosition( { pos.x, pos.y } );

	m_spriteSlider = std::make_shared<Sprite>();
    m_spriteSlider->SetWidthHeight( size.x, size.y );
    m_transformSlider = std::make_shared<Transform>( m_spriteSlider );
    m_transformSlider->SetPosition( { pos.x, pos.y } );

	int sliderStart = 50;
	MouseData mouseData = MouseData();
	std::string texture = "Resources\\Textures\\Tiles\\empty.png";
	Resolve( sliderStart, texture, texture, mouseData );
}

DataSlider_Widget::DataSlider_Widget( int start, const std::string& barTex, const std::string& sliderTex, MouseData& mData, XMFLOAT2 pos, XMFLOAT2 size )
{
	m_spriteBar = std::make_shared<Sprite>();
    m_spriteBar->SetWidthHeight( size.x, size.y );
    m_transformBar = std::make_shared<Transform>( m_spriteBar );
    m_transformBar->SetPosition( { pos.x, pos.y } );

	m_spriteSlider = std::make_shared<Sprite>();
    m_spriteSlider->SetWidthHeight( size.x, size.y );
    m_transformSlider = std::make_shared<Transform>( m_spriteSlider );
    m_transformSlider->SetPosition( { pos.x, pos.y } );

	Resolve( start, barTex, sliderTex, mData );
}

DataSlider_Widget::~DataSlider_Widget() { }

void DataSlider_Widget::Initialize( ID3D11Device* device, ID3D11DeviceContext* context, ConstantBuffer<Matrices>& mat )
{
	m_spriteBar->Initialize( device, context, m_barTexture, mat );
	m_spriteSlider->Initialize( device, context, m_sliderTexture, mat );
}

void DataSlider_Widget::Update( const float dt )
{
	m_transformBar->Update();
	m_transformSlider->Update();
}

void DataSlider_Widget::Draw( ID3D11Device* device, ID3D11DeviceContext* context, XMMATRIX worldOrtho )
{
	// Bar
	m_spriteBar->UpdateTex( device, m_barTexture );
	m_spriteBar->UpdateBuffers( context );
	m_spriteBar->Draw( m_transformBar->GetWorldMatrix(), worldOrtho );

	// Slider
	m_spriteSlider->UpdateTex( device, m_sliderTexture );
	m_spriteSlider->UpdateBuffers( context );
	m_spriteSlider->Draw( m_transformSlider->GetWorldMatrix(), worldOrtho );
}

void DataSlider_Widget::Resolve( int& start, const std::string& barTex, const std::string& sliderTex, MouseData& mData )
{
	m_barTexture = barTex;
	m_sliderTexture = sliderTex;
	m_fPx = ( ( float )start / 100.0f ) * m_spriteBar->GetWidth();

	m_transformSlider->SetPosition( ( m_transformBar->GetPosition().x + m_fPx ) - 25.0f / 2.0f,
		m_transformBar->GetPosition().y + ( 30.0f - ( 30.0f / 0.75f ) ) );
	m_spriteSlider->SetWidthHeight( 25.0f, m_spriteBar->GetHeight() / 0.75f );

	// Slider collision
	if (
		mData.Pos.x >= m_transformBar->GetPosition().x &&
		mData.Pos.x <= ( m_transformBar->GetPosition().x + m_spriteBar->GetWidth() + 1.0f ) &&
		mData.Pos.y >= m_transformBar->GetPosition().y &&
		mData.Pos.y <= ( m_transformBar->GetPosition().y + m_spriteBar->GetHeight() )
	   )
	{
		if ( mData.LPress )
		{
			mData.Locked = true;
			m_fPx = mData.Pos.x - m_transformBar->GetPosition().x;
			start = ( m_fPx / m_spriteBar->GetWidth() ) * 100.0f;
		}
	}

	m_uDataOut = ( m_fPx / m_spriteBar->GetWidth() ) * 100.0f;
}