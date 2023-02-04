#pragma once
#ifndef INPUT_WIDGET
#define INPUT_WIDGET

#include "MouseData.h"
#include "Transform.h"
#include "TextRenderer.h"

class Input_Widget
{
public:
    Input_Widget( XMFLOAT2 pos, XMFLOAT2 size );
    ~Input_Widget();

    void Initialize( ID3D11Device* device, ID3D11DeviceContext* context, ConstantBuffer<Matrices>& mat, int index );
    void Update( const float dt );
    void Draw( ID3D11Device* device, ID3D11DeviceContext* context, XMMATRIX worldOrtho, TextRenderer* textRenderer );
    void Resolve( std::string& keys, XMVECTORF32 textColour, const std::vector<std::string>& textures, MouseData& mData, int index );

    inline bool GetSelected() const noexcept { return m_bSelected; }
    inline void SetSelected( bool selected ) noexcept { m_bSelected = selected; }

    void SetCurrentText( std::string text ) noexcept { m_sCurrText = text; }
    std::string GetCurrentText() const noexcept { return m_sCurrText; }

    inline std::shared_ptr<Sprite> GetSprite() const noexcept { return m_sprite; }
    inline std::shared_ptr<Transform> GetTransform() const noexcept { return m_transform; }

private:
    bool m_bSelected = false;
    std::string m_sCurrText;
    int m_iIdentifier;

    XMVECTORF32 m_textColour;
    std::string m_buttonTexture;

    std::shared_ptr<Sprite> m_sprite;
    std::shared_ptr<Transform> m_transform;
};

#endif