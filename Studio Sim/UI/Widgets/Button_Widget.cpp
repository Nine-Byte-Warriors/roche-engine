#include "stdafx.h"
#include "Button_Widget.h"
#include "TextRenderer.h"

Button_Widget::Button_Widget()
{    
    m_sprite = std::make_shared<Sprite>();
    m_transform = std::make_shared<Transform>( m_sprite );

    std::string texture = "Resources\\Textures\\empty.png";
    std::vector<std::string> buttonTextures = { texture, texture, texture };
    Resolve( "Default", Colors::Black, buttonTextures, {}, { 0.0f, 0.0f }, { 64.0f, 64.0f } );
}

Button_Widget::Button_Widget( const std::string& texture, XMFLOAT2 pos, XMFLOAT2 size )
{
	m_sprite = std::make_shared<Sprite>();
	m_transform = std::make_shared<Transform>( m_sprite );

    std::vector<std::string> buttonTextures = { texture, texture, texture };
    Resolve( "Default", Colors::Black, buttonTextures, {}, pos, size );
}

Button_Widget::~Button_Widget() { }

void Button_Widget::Initialize( ID3D11Device* device, ID3D11DeviceContext* context, ConstantBuffer<Matrices>& mat )
{
	m_sprite->Initialize( device, context, "", mat, m_vSize.x, m_vSize.y );
    m_transform->SetPositionInit( m_vPosition.x, m_vPosition.y );
	m_transform->SetScaleInit( m_vSize.x, m_vSize.y );
}

void Button_Widget::Update( const float dt )
{
    m_sprite->Update( dt );
	m_transform->Update();
}

void Button_Widget::Draw( ID3D11Device* device, ID3D11DeviceContext* context, XMMATRIX worldOrtho, TextRenderer* textRenderer )
{
    // Button sprite
    m_sprite->UpdateTex( device, m_buttonTexture );
    m_sprite->UpdateBuffers( context );
    m_sprite->Draw( m_transform->GetWorldMatrix(), worldOrtho );

    // Button text
    XMVECTOR textsize = textRenderer->GetSpriteFont()->MeasureString( m_sText.c_str() );
    XMFLOAT2 textpos =
    {
        m_transform->GetPosition().x + ( m_transform->GetScale().x / 2.0f ) - ( XMVectorGetX( textsize ) * textRenderer->GetScale().x ) / 2.0f,
        m_transform->GetPosition().y + ( m_transform->GetScale().y / 2.0f ) - ( XMVectorGetY( textsize ) * textRenderer->GetScale().y ) / 2.0f
    };
    textRenderer->RenderString( m_sText, textpos, m_vTextColor, false );
}

bool Button_Widget::Resolve( const std::string& text, XMVECTORF32 textColour, std::vector<std::string> textures, MouseData mData, XMFLOAT2 pos, XMFLOAT2 size )
{
    m_sText = text;
    m_vTextColor = textColour;

    // Update position/scale
    m_vSize = size;
    m_vPosition = pos;

    m_transform->SetPosition( m_vPosition.x, m_vPosition.y );
    m_transform->SetScale( m_vSize.x, m_vSize.y );

    m_sprite->SetWidth( m_vSize.x );
    m_sprite->SetHeight( m_vSize.y );

#if !_DEBUG // not updated for imgui mouse positions
    // Button collison
    m_buttonState = ButtonState::Default;
    if (
        mData.Pos.x >= m_transform->GetPosition().x &&
        mData.Pos.x <= ( m_transform->GetPosition().x + m_transform->GetScale().x ) &&
        mData.Pos.y >= m_transform->GetPosition().y &&
        mData.Pos.y <= ( m_transform->GetPosition().y + m_transform->GetScale().y )
       )
    {
    	if ( mData.LPress )
    		m_buttonState = ButtonState::Pressed;
    	else 
            m_buttonState = ButtonState::Hover;
    }
#endif

    // Button state
    switch ( m_buttonState )
    {
    case ButtonState::Default:
        m_buttonTexture = textures[0];
        break;
    case ButtonState::Hover:
        m_buttonTexture = textures[1];
        break;
    case ButtonState::Pressed:
        m_buttonTexture = textures[2];
        m_bIsPressed = true;
    	return true;
    default:
    	break;
    }
    m_bIsPressed = false;
    return false;
}