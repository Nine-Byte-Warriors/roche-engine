#pragma once
#ifndef INPUT_WIDGET
#define INPUT_WIDGET

#include "Widget.h"
#include "Transform.h"

class Input_Widget : public Widget
{
public:
    Input_Widget();
    Input_Widget( const std::string& texture, XMFLOAT2 pos, XMFLOAT2 size );
    ~Input_Widget();

    void Initialize( ID3D11Device* device, ID3D11DeviceContext* context, ConstantBuffer<Matrices>& mat );
    void Update( const float dt );
    void Draw( ID3D11Device* device, ID3D11DeviceContext* context, XMMATRIX worldOrtho, TextRenderer* textRenderer );
    void Resolve( std::string& keys, XMVECTORF32 textColour, const std::vector<std::string>& textures, MouseData mData, XMFLOAT2 pos, XMFLOAT2 size );
    
    void SetCurrentText( std::string text ) noexcept { m_sCurrText = text; }
    std::string GetCurrentText() const noexcept { return m_sCurrText; }

private:
    std::string m_sCurrText;
    bool m_bSelected = false;

    XMVECTORF32 m_textColour;
    std::string m_buttonTexture;
    XMFLOAT2 m_vPosition, m_vSize;

    std::shared_ptr<Sprite> m_sprite;
    std::shared_ptr<Transform> m_transform;
};

#endif