#include "stdafx.h"
#include "PageSlider_Widget.h"

PageSlider_Widget::PageSlider_Widget()
{
	m_spriteBar = std::make_shared<Sprite>();
    m_transformBar = std::make_shared<Transform>( m_spriteBar );

	m_spriteSlider = std::make_shared<Sprite>();
    m_transformSlider = std::make_shared<Transform>( m_spriteSlider );

	MouseData mouseData = MouseData();
	Resolve( Colour( 10.0f, 10.0f, 10.0f ), Colour( 60.0f, 60.0f, 60.0f ), mouseData );
}

PageSlider_Widget::PageSlider_Widget( Colour barCol, Colour sliderCol, MouseData& mData )
{
	m_spriteBar = std::make_shared<Sprite>();
    m_transformBar = std::make_shared<Transform>( m_spriteBar );

	m_spriteSlider = std::make_shared<Sprite>();
    m_transformSlider = std::make_shared<Transform>( m_spriteSlider );

	Resolve( barCol, sliderCol, mData );
}

PageSlider_Widget::~PageSlider_Widget() { }

void PageSlider_Widget::Initialize( ID3D11Device* device, ID3D11DeviceContext* context, ConstantBuffer<Matrices>& mat )
{	
	m_spriteBar->Initialize( device, context, m_barColour, mat, m_vSize.x, m_vSize.y );
    m_transformBar->SetPositionInit( m_vPosition.x, m_vPosition.y );
	m_transformBar->SetScaleInit( m_vSize.x, m_vSize.y );

	m_spriteSlider->Initialize( device, context, m_sliderColour, mat, m_vSize.x, m_vSize.y );
    m_transformSlider->SetPositionInit( m_vPosition.x, m_vPosition.y );
	m_transformSlider->SetScaleInit( m_vSize.x, m_vSize.y );
}

void PageSlider_Widget::Update( const float dt )
{
	m_spriteBar->Update( dt );
	m_transformBar->Update();

	m_spriteSlider->Update( dt );
	m_transformSlider->Update();
}

void PageSlider_Widget::Draw( ID3D11Device* device, ID3D11DeviceContext* context, XMMATRIX worldOrtho )
{
	// Bar
	m_spriteBar->UpdateTex( device, m_barColour );
	m_spriteBar->UpdateBuffers( context );
	m_spriteBar->Draw( m_transformBar->GetWorldMatrix(), worldOrtho );

	// Slider
	m_spriteSlider->UpdateTex( device, m_sliderColour );
	m_spriteSlider->UpdateBuffers( context );
	m_spriteSlider->Draw( m_transformSlider->GetWorldMatrix(), worldOrtho );
}

void PageSlider_Widget::Resolve( Colour barCol, Colour sliderCol, MouseData& mData )
{	
	m_barColour = barCol;
	m_sliderColour = sliderCol;

	m_transformSlider->SetScale( m_vSize.x, m_vSize.y * 0.2f );
	m_spriteSlider->SetWidth( m_vSize.x );
	m_spriteSlider->SetHeight( m_vSize.y * 0.2f );
	m_transformSlider->SetPosition( m_vPosition.x, m_vPosition.y + m_fPY - ( m_spriteSlider->GetHeight() / 2.0f ) );

	m_transformBar->SetPosition( m_vPosition.x, m_vPosition.y - ( m_spriteSlider->GetHeight() / 2.0f ) );
	m_transformBar->SetScale( m_vSize.x, m_vSize.y + 85.0f );
	m_spriteBar->SetWidth( m_vSize.x );
	m_spriteBar->SetHeight( m_vSize.y + 85.0f );

#if !_DEBUG // not updated for imgui mouse positions
	// Slider collision
	if (
		mData.Pos.x >= m_vPosition.x &&
		mData.Pos.x <= ( m_vPosition.x + m_vSize.x ) &&
		mData.Pos.y >= m_vPosition.y &&
		mData.Pos.y <= ( m_vPosition.y + m_vSize.y )
	   )
	{
		if ( mData.LPress )
		{
			mData.Locked = true;
			m_fPY = mData.Pos.y - ( m_vPosition.y );
		}
	}
#endif

	m_fPagePos = m_fPageSize * ( m_fPY / m_vSize.y );
}