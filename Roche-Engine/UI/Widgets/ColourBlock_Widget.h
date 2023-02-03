#pragma once
#ifndef COLOURBLOCK_WIDGET_H
#define COLOURBLOCK_WIDGET_H

#include "Transform.h"

class ColourBlock_Widget
{
public:
    ColourBlock_Widget( XMFLOAT2 pos, XMFLOAT2 size );
    ColourBlock_Widget( Colour colour, XMFLOAT2 pos, XMFLOAT2 size );
    ~ColourBlock_Widget();

    void Initialize( ID3D11Device* device, ID3D11DeviceContext* context, ConstantBuffer<Matrices>& mat );
    void Update( const float dt );
    void Draw( ID3D11Device* device, ID3D11DeviceContext* context, XMMATRIX worldOrtho );
    void Resolve( Colour colour );

    inline std::shared_ptr<Sprite> GetSprite() const noexcept { return m_sprite; }
    inline std::shared_ptr<Transform> GetTransform() const noexcept { return m_transform; }

private:
    Colour m_colour;
    std::shared_ptr<Sprite> m_sprite;
    std::shared_ptr<Transform> m_transform;
};

#endif