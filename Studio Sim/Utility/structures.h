#pragma once
#ifndef STRUCTURES_H
#define STRUCTURES_H

using namespace DirectX;

struct Matrices
{
	XMMATRIX wvpMatrix;
};

struct RenderToTexture
{
	XMFLOAT3 OverlayColor;
	FLOAT Padding;
};

struct Animation
{
	FLOAT Width;
	FLOAT Height;
	int Rows;
	int Columns;

	int FrameX;
	int FrameY;
	XMFLOAT2 Padding;
};

struct UserInterface
{
	FLOAT Alpha;
	XMFLOAT3 Padding;
};

#endif