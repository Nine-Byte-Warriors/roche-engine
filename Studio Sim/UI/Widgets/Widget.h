#pragma once
#ifndef WIDGET_H
#define WIDGET_H

#include "Sprite.h"

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
	virtual void Draw( ID3D11Device* device, const ConstantBuffer<Matrices>& mat, XMMATRIX worldOrthoMat, TextRenderer* textRenderer ) {}	
};

#endif