#pragma once
#ifndef WIDGET_H
#define WIDGET_H

#include "Sprite.h"
#include "TextRenderer.h"

struct MouseData
{
	XMFLOAT2 Pos;
	bool LPress;
	bool RPress;
	bool MPress;
};

class Widget
{
public:
	Widget() {}
	~Widget() {}

	virtual void Resolve() {}
	virtual void Draw( ID3D11Device* device, ID3D11DeviceContext* context, XMMATRIX worldOrtho ) {}
};

#endif