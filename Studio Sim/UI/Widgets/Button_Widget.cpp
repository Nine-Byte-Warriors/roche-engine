#include "stdafx.h"
#include "Button_Widget.h"
#include "TextRenderer.h"
#include "Graphics.h"

template<typename ButtonTexture>
Button_Widget<ButtonTexture>::Button_Widget()
{
    m_vSize = { 1.0f, 1.0f };
    m_vPosition = { 0.0f, 0.0f };
    m_buttonTexture = "";
    
    m_sprite = std::make_shared<Sprite>();
    m_transform = std::make_shared<Transform>( m_sprite );
}

template<typename ButtonTexture>
Button_Widget<ButtonTexture>::Button_Widget( ButtonTexture texture, XMFLOAT2 pos, XMFLOAT2 size )
{
    m_vSize = size;
    m_vPosition = pos;
    m_buttonTexture = texture;

	m_sprite = std::make_shared<Sprite>();
	m_transform = std::make_shared<Transform>( m_sprite );
}

template<typename ButtonTexture>
Button_Widget<ButtonTexture>::~Button_Widget() { }

template<typename ButtonTexture>
void Button_Widget<ButtonTexture>::Initialize( const Graphics& gfx, ConstantBuffer<Matrices>& mat )
{
	m_sprite->Initialize( gfx.GetDevice(), gfx.GetContext(), "", mat );
    m_transform->SetPositionInit( m_vPosition.x, m_vPosition.y );
	m_transform->SetScaleInit( m_vSize.x, m_vSize.y );
}

template<typename ButtonTexture>
void Button_Widget<ButtonTexture>::Update( const float dt )
{
    m_sprite->Update( dt );
	m_transform->Update();
}

template<typename ButtonTexture>
void Button_Widget<ButtonTexture>::Draw( ID3D11Device* device, ConstantBuffer<Matrices>& mat, XMMATRIX worldOrtho, TextRenderer* textRenderer )
{
    // Button sprite
    m_sprite->UpdateTex( device, m_buttonTexture );
    m_sprite->UpdateBuffers( device );
    m_sprite->Draw( worldOrtho );

    // Button text
    XMVECTOR textsize = textRenderer->GetSpriteFont()->MeasureString( m_sText.c_str() );
    XMFLOAT2 textpos =
    {
        m_transform->GetPosition().x + ( m_transform->GetScale().x / 2.0f ) - ( XMVectorGetX( textsize ) * textRenderer->GetScale().x ) / 2.0f,
        m_transform->GetPosition().y + ( m_transform->GetScale().y / 2.0f ) - ( XMVectorGetY( textsize ) * textRenderer->GetScale().y ) / 2.0f
    };
    textRenderer->RenderString( m_sText, textpos, m_vTextColor, false );
}

template<typename ButtonTexture>
bool Button_Widget<ButtonTexture>::Resolve( const std::string& text, XMVECTORF32 textColour, const std::vector<ButtonTexture>& textures, MouseData mData )
{
    m_sText = text;
    m_vTextColor = textColour;
    m_buttonState = ButtonState::Default;

    // Button collison
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

    // Button state
    switch ( m_buttonState )
    {
    case ButtonState::Default:
        m_buttonTexture = textures[0];
        break;
    case ButtonState::Pressed:
        m_buttonTexture = textures[1];
        m_bIsPressed = true;
    	return true;
    case ButtonState::Hover:
        m_buttonTexture = textures[2];
        break;
    default:
    	break;
    }
    m_bIsPressed = false;
    return false;
}