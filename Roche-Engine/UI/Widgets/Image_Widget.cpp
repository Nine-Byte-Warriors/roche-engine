#include "stdafx.h"
#include "Image_Widget.h"
#include "Graphics.h"

Image_Widget::Image_Widget()
{
	m_sprite = std::make_shared<Sprite>();
	m_transform = std::make_shared<Transform>( m_sprite );
	Resolve("", Colors::White, "Resources\\Textures\\Tiles\\empty.png" );
}

Image_Widget::Image_Widget( const std::string& texture )
{
	m_sprite = std::make_shared<Sprite>();
	m_transform = std::make_shared<Transform>( m_sprite );
	Resolve("", Colors::White,texture );
}

Image_Widget::~Image_Widget() { }

void Image_Widget::Initialize( ID3D11Device* device, ID3D11DeviceContext* context, ConstantBuffer<Matrices>& mat )
{
	m_sprite->Initialize( device, context, m_sTextFile, mat, m_vSize.x, m_vSize.y );
	m_transform->SetPositionInit( m_vPosition.x, m_vPosition.y );
	m_transform->SetScaleInit( m_vSize.x, m_vSize.y );
}

void Image_Widget::Update( const float dt )
{
	m_sprite->Update( dt );
	m_transform->Update();
}

void Image_Widget::Draw( ID3D11Device* device, ID3D11DeviceContext* context, XMMATRIX worldOrtho, TextRenderer* textRenderer)
{
	m_sprite->UpdateTex( device, m_sTextFile );
	m_sprite->UpdateBuffers( context );
	m_sprite->Draw( m_transform->GetWorldMatrix(), worldOrtho );

	// image text
	XMVECTOR textsize = textRenderer->GetSpriteFont()->MeasureString(m_sText.c_str());
	XMFLOAT2 textpos =
	{
		m_transform->GetPosition().x + (m_transform->GetScale().x / 2.0f) - (XMVectorGetX(textsize) * textRenderer->GetScale().x) / 2.0f,
		m_transform->GetPosition().y + (m_transform->GetScale().y / 2.0f) - (XMVectorGetY(textsize) * textRenderer->GetScale().y) / 2.0f
	};
	textRenderer->RenderString(m_sText, textpos, m_vTextColor, false);
}

void Image_Widget::Resolve(const std::string& text, XMVECTORF32 textColour, const std::string& texture )
{
	m_sText = text;
	m_vTextColor = textColour;

	m_transform->SetPosition( m_vPosition.x, m_vPosition.y );
	m_transform->SetScale( m_vSize.x, m_vSize.y );
	
	m_sprite->SetWidth( m_vSize.x );
	m_sprite->SetHeight( m_vSize.y );

	if ( m_sTextFile != texture )
		m_sTextFile = texture;
}