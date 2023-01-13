#include "stdafx.h"
#include "Button_Widget.h"
#include "TextRenderer.h"
#include "Graphics.h"

template<typename ButtonTexture>
Button_Widget<ButtonTexture>::Button_Widget()
{
    m_sprite = std::make_shared<Sprite>();
    m_transform = std::make_shared<Transform>( m_sprite );
}

template<typename ButtonTexture>
void Button_Widget<ButtonTexture>::Initialize( const Graphics& gfx, const ConstantBuffer<Matrices>& mat )
{
	m_sprite->Initialize( gfx.GetDevice(), gfx.GetContext(), Sprite::Type::Player, mat );
}

template<typename ButtonTexture>
void Button_Widget<ButtonTexture>::Update( const float dt )
{
    m_sprite->Update( dt );
	m_transform->Update();
}

template<typename ButtonTexture>
void Button_Widget<ButtonTexture>::Draw( ID3D11Device* device, const ConstantBuffer<Matrices>& mat, XMMATRIX worldOrthoMat, TextRenderer* textRenderer )
{
    // Button sprite
    m_sprite->UpdateTex( device, m_buttonTexture );
    m_sprite->Draw( worldOrthoMat );

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
        mData.Pos.x >= pos.x &&
        mData.Pos.x <= (pos.x + size.x) &&
        mData.Pos.y >= pos.y &&
        mData.Pos.y <= (pos.y + size.y)
       )
    {
    	if ( mData.LPress )
    		CurrentState = Pressed;
    	else 
    		CurrentState = Hover;
    }

    // Button state
    switch ( m_buttonState )
    {
    case ButtonState::Default:
        ButtonColour = ButtonText[0];
        break;
    case ButtonState::Pressed:
    	ButtonColour = ButtonText[1];
    	IsPressed = true;
    	return true;
    case ButtonState::Hover:
        ButtonColour = ButtonText[2];
        break;
    default:
    	break;
    }
    IsPressed = false;
    return false;
}