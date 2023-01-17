#include "stdafx.h"
#include "DataSlider_Widget.h"

DataSlider_Widget::DataSlider_Widget()
{
	m_spriteBar = std::make_shared<Sprite>();
    m_transformBar = std::make_shared<Transform>( m_spriteBar );

	m_spriteSlider = std::make_shared<Sprite>();
    m_transformSlider = std::make_shared<Transform>( m_spriteSlider );

	int sliderStart = 50;
	std::string texture = "Resources\\Textures\\Tiles\\empty.png";
	Resolve( sliderStart, texture, texture, {}, { 0.0f, 0.0f }, { 64.0f, 64.0f } );
}

DataSlider_Widget::DataSlider_Widget( int start, const std::string& barTex, const std::string& sliderTex, MouseData mData, XMFLOAT2 pos, XMFLOAT2 size )
{
	m_spriteBar = std::make_shared<Sprite>();
    m_transformBar = std::make_shared<Transform>( m_spriteBar );

	m_spriteSlider = std::make_shared<Sprite>();
    m_transformSlider = std::make_shared<Transform>( m_spriteSlider );

	Resolve( start, barTex, sliderTex, mData, pos, size );
}

DataSlider_Widget::~DataSlider_Widget() { }

void DataSlider_Widget::Initialize( ID3D11Device* device, ID3D11DeviceContext* context, ConstantBuffer<Matrices>& mat )
{
	m_spriteBar->Initialize( device, context, m_barTexture, mat, m_vSize.x, m_vSize.y );
    m_transformBar->SetPositionInit( m_vPosition.x, m_vPosition.y );
	m_transformBar->SetScaleInit( m_vSize.x, m_vSize.y );

	m_spriteSlider->Initialize( device, context, m_sliderTexture, mat, m_vSize.x, m_vSize.y );
    m_transformSlider->SetPositionInit( m_vPosition.x, m_vPosition.y );
	m_transformSlider->SetScaleInit( m_vSize.x, m_vSize.y );
}

void DataSlider_Widget::Update( const float dt )
{
	m_spriteBar->Update( dt );
	m_transformBar->Update();

	m_spriteSlider->Update( dt );
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

void DataSlider_Widget::Resolve( int& start, const std::string& barTex, const std::string& sliderTex, MouseData mData, XMFLOAT2 pos, XMFLOAT2 size )
{
	m_barTexture = barTex;
	m_sliderTexture = sliderTex;
	m_fPx = ( ( float )start / 100.0f ) * size.x;

	// Update position/scale
    m_vSize = size;
    m_vPosition = pos;

    m_transformBar->SetPosition( m_vPosition.x, m_vPosition.y );
    m_transformBar->SetScale( m_vSize.x, m_vSize.y );
    m_spriteBar->SetWidth( m_vSize.x );
    m_spriteBar->SetHeight( m_vSize.y );

	m_transformSlider->SetPosition( ( m_vPosition.x + m_fPx ) - 25.0f / 2.0f, m_vPosition.y + ( 30.0f - ( 30.0f / 0.75f ) ) );
	m_transformSlider->SetScale( 25.0f, m_vSize.y / 0.75f );
	m_spriteSlider->SetWidth( 25.0f );
	m_spriteSlider->SetHeight( m_vSize.y / 0.75f );

#if !_DEBUG // not updated for imgui mouse positions
	// Slider collision
	if (
		mData.Pos.x >= pos.x &&
		mData.Pos.x <= ( pos.x + size.x + 1.0f ) &&
		mData.Pos.y >= pos.y &&
		mData.Pos.y <= ( pos.y + size.y )
	   )
	{
		if ( mData.LPress )
		{
			m_fPx = mData.Pos.x - pos.x;
			start = ( m_fPx / size.x ) * 100.0f;
		}
	}
#endif

	m_uDataOut = ( m_fPx / size.x ) * 100.0f;
}