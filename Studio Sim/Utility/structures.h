#pragma once
#ifndef STRUCTURES_H
#define STRUCTURES_H

using namespace DirectX;

// Matrices
struct Matrices
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
};

#endif