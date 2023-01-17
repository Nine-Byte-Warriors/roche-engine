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
    void Resolve( unsigned char key, XMVECTORF32 textColour, const std::string& backImg, MouseData mData, XMFLOAT2 pos, XMFLOAT2 size );
    
    std::string GetCurrentText() const noexcept { return m_sCurrText; }
    void SetCurrentText( std::string text ) noexcept { m_sCurrText = text; }

	void SetKey( unsigned char key );
	unsigned char GetKey() const noexcept { return m_cKey; }

private:
    unsigned char m_cKey;
    std::string m_sCurrText;
    bool m_bSelected = false;

    XMVECTORF32 m_textColour;
    std::string m_textureBack;
    XMFLOAT2 m_vPosition, m_vSize;

    std::shared_ptr<Sprite> m_sprite;
    std::shared_ptr<Transform> m_transform;
};

#endif