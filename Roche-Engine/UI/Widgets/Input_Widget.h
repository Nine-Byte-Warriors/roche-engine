#pragma once
#ifndef INPUT_WIDGET
#define INPUT_WIDGET

#include "MouseData.h"
#include "Transform.h"
#include "TextRenderer.h"

class Input_Widget
{
public:
    Input_Widget();
    ~Input_Widget();

    void Initialize( ID3D11Device* device, ID3D11DeviceContext* context, ConstantBuffer<Matrices>& mat, int index );
    void Update( const float dt );
    void Draw( ID3D11Device* device, ID3D11DeviceContext* context, XMMATRIX worldOrtho, TextRenderer* textRenderer );
    void Resolve( std::string placeholder, std::string& keys, XMVECTORF32 textColour, const std::vector<std::string>& textures, MouseData& mData, int index, FontSize size = FontSize::MEDIUM );

    inline bool GetSelected() const noexcept { return m_bSelected; }
    inline void SetSelected( bool selected ) noexcept { m_bSelected = selected; }
    inline bool GetIsUsingPlaceholder() const noexcept { return m_bUsePlaceholder; }

    void SetCurrentText( std::string text ) noexcept { m_sCurrText = text; }
    std::string GetCurrentText() const noexcept { return m_sCurrText.empty() ? m_sPlaceholder : m_sCurrText; }
    inline void SetTextOffset( XMFLOAT2 pos ) noexcept { m_vTextOffset = pos; }

    inline std::shared_ptr<Sprite> GetSprite() const noexcept { return m_sprite; }
    inline std::shared_ptr<Transform> GetTransform() const noexcept { return m_transform; }

private:
    FontSize m_eFontSize = FontSize::MEDIUM;
    bool m_bSelected = false;
    int m_iIdentifier;

    bool m_bUsePlaceholder = true;
    std::string m_sPlaceholder;

    XMFLOAT2 m_vTextOffset;
    std::string m_sCurrText;
    XMVECTORF32 m_textColour;
    std::string m_buttonTexture;

    std::shared_ptr<Sprite> m_sprite;
    std::shared_ptr<Transform> m_transform;
};

#endif