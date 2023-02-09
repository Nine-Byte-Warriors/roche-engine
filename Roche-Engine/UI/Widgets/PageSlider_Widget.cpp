#include "stdafx.h"
#include "PageSlider_Widget.h"

PageSlider_Widget::PageSlider_Widget()
{
	m_spriteBar = std::make_shared<Sprite>();
	m_transformBar = std::make_shared<Transform>( m_spriteBar );

	m_spriteSlider = std::make_shared<Sprite>();
	m_transformSlider = std::make_shared<Transform>( m_spriteSlider );
}

PageSlider_Widget::~PageSlider_Widget() { }

void PageSlider_Widget::Initialize( ID3D11Device* device, ID3D11DeviceContext* context, ConstantBuffer<Matrices>& mat, float pageSizeX, float pageSizeY )
{
	m_fPageSizeX = pageSizeX;
	m_fPageSizeY = pageSizeY;
	m_spriteBar->Initialize( device, context, m_barColour, mat );
	m_spriteSlider->Initialize( device, context, m_sliderColour, mat );
}

void PageSlider_Widget::Update( const float dt )
{
	m_transformBar->Update();
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

	m_transformBar->SetPosition( m_transformBar->GetPosition().x, m_transformBar->GetPosition().y - ( m_spriteSlider->GetHeight() / 2.0f ) );
	m_spriteBar->SetHeight( m_spriteBar->GetHeight() + 85.0f );

	m_spriteSlider->SetHeight( m_spriteBar->GetHeight() * 0.2f );
	m_transformSlider->SetPosition( m_transformBar->GetPosition().x, m_transformBar->GetPosition().y + m_fPY - ( m_spriteSlider->GetHeight() / 2.0f ) );

	// Slider collision
	if (
		mData.Pos.x >= m_transformBar->GetPosition().x &&
		mData.Pos.x <= ( m_transformBar->GetPosition().x + m_spriteBar->GetWidth() ) &&
		mData.Pos.y >= m_transformBar->GetPosition().y &&
		mData.Pos.y <= ( m_transformBar->GetPosition().y + m_spriteBar->GetHeight() )
	   )
	{
		EventSystem::Instance()->AddEvent( EVENTID::CursorUpdate_Move );
		if ( mData.LPress )
		{
			mData.Locked = true;
			m_fPY = mData.Pos.y - ( m_transformBar->GetPosition().y );
		}
	}

	m_fPagePos = m_fPageSizeY * ( m_fPY / m_transformBar->GetPosition().y );
}