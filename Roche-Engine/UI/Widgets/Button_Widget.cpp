#include "stdafx.h"
#include "Button_Widget.h"
#include "AudioEngine.h"

#define SOUND_BANK_NAME "UI"

Button_Widget::Button_Widget()
{
    m_sprite = std::make_shared<Sprite>();
    m_transform = std::make_shared<Transform>( m_sprite );
}

Button_Widget::~Button_Widget() { }

void Button_Widget::Initialize( ID3D11Device* device, ID3D11DeviceContext* context, ConstantBuffer<Matrices>& mat )
{
	m_sprite->Initialize( device, context, "", mat );
}

void Button_Widget::Update( const float dt )
{
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
        m_transform->GetPosition().x + ( m_sprite->GetWidth() / 2.0f ) - ( XMVectorGetX( textsize ) * textRenderer->GetScale().x ) / 2.0f,
        m_transform->GetPosition().y + ( m_sprite->GetHeight() / 2.0f ) - ( XMVectorGetY( textsize ) * textRenderer->GetScale().y ) / 2.0f
    };
    textRenderer->RenderString( m_sText, textpos, m_vTextColor, false );
}

bool Button_Widget::Resolve( const std::string& text, XMVECTORF32 textColour, const std::vector<std::string>& textures, MouseData& mData )
{
    m_sText = text;
    m_vTextColor = textColour;
    m_buttonState = ButtonState::Default;

    // Button collison
    if (
        mData.Pos.x >= m_transform->GetPosition().x &&
        mData.Pos.x <= ( m_transform->GetPosition().x + m_sprite->GetWidth() ) &&
        mData.Pos.y >= m_transform->GetPosition().y &&
        mData.Pos.y <= ( m_transform->GetPosition().y + m_sprite->GetHeight() )
       )
    {
    	if ( mData.LPress && !mData.Locked )
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
    case ButtonState::Hover:
        m_buttonTexture = textures[1];
        break;
    case ButtonState::Pressed:
        m_buttonTexture = textures[2];
        AudioEngine::GetInstance()->PlayAudio(SOUND_BANK_NAME, "ButtonPress", SFX);
        m_bIsPressed = true;
        mData.Locked = true;
    	return true;
    default:
    	break;
    }
    m_bIsPressed = false;
    return false;
}