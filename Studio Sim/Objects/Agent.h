#pragma once
#ifndef AGENT_H
#define AGENT_H

#include "GameObject.h"
#include "ConstantBuffer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "structures.h"
#include "Texture.h"
#include "Sprite.h"

#include "AIStateMachine.h"

#include <vector>

namespace AILogic
{
	class AIState;
}

/// <summary>
/// Create a 2D agent object.
/// Inherits from GameObject2D.h to allow for modification of position/rotation/scale data in 2D space.
/// </summary>
class Agent : public GameObject
{
public:
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* context,
		float width, float height, std::string spritePath,
		ConstantBuffer<Matrices2D>& cb_vs_vertexshader_2d);
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* context,
		float width, float height, Colour spriteColour,
		ConstantBuffer<Matrices2D>& cb_vs_vertexshader_2d);

	void Draw(XMMATRIX orthoMatrix);
	void UpdateBuffers(ID3D11DeviceContext* context);

	float GetWidth() const noexcept { return scale.x; }
	float GetHeight() const noexcept { return scale.y; }

	void UpdateTex(ID3D11Device* device, std::string tex) { texture->UpdateTexture(device, tex); }
	void UpdateTex(ID3D11Device* device, Colour tex) { texture->UpdateTexture(device, tex); }

	void InitialiseAILogic();
	Vector2f GetPositionVector2f() const noexcept { XMFLOAT3 pos = GetPositionFloat3();  return Vector2f(pos.x, pos.y); }
	void SetVelocity(const Vector2f vel) { m_vVelocity = vel; }
	void Update(float dt);
	
private:
	void UpdateMatrix() override;
	std::unique_ptr<Texture> texture;
	XMMATRIX worldMatrix = XMMatrixIdentity();

	ConstantBuffer<Matrices2D>* cb_vs_matrix_2d = nullptr;
	ConstantBuffer<Animation> m_cbAnimation;
	ID3D11DeviceContext* context = nullptr;
	VertexBuffer<Vertex2D> vertices;
	IndexBuffer indices;
	Colour colour;

	AILogic::AIStateMachine* m_pStateMachine;
	std::vector<AILogic::AIState*> m_vecStates;
	Vector2f m_vVelocity;
};

#endif