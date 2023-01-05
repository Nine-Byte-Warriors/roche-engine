#pragma once
#ifndef STRUCTURES_H
#define STRUCTURES_H

using namespace DirectX;

struct Matrices
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
};

struct RenderToTexture
{
	XMFLOAT3 OverlayColor;
	FLOAT Padding;
};

#endif