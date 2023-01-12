#pragma once
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "Quad.h"
#include "Shaders.h"
#include "Sampler.h"
#include "Viewport.h"
#include "SwapChain.h"
#include "Rasterizer.h"
#include "BackBuffer.h"
#include "DepthStencil.h"
#include "RenderTarget.h"
#include "ConstantBuffer.h"
#include "EventSystem.h"
#include "structures.h"

class Graphics : public Listener
{
public:
	bool Initialize( HWND hWnd, UINT width, UINT height );
	void UpdateRenderState();
	
	void BeginRTT();
	void RenderRTT();
	void EndRTT();
	void SpawnControlWindowRTT();

	void BeginFrame();
	void EndFrame();

	inline UINT GetWidth() const noexcept { return m_viewWidth; }
	inline UINT GetHeight() const noexcept { return m_viewHeight; }
	
	inline ID3D11Device* GetDevice() const noexcept { return m_pDevice.Get(); }
	inline ID3D11DeviceContext* GetContext() const noexcept { return m_pContext.Get(); }
	
	inline Bind::BackBuffer* GetBackBuffer() const noexcept { return &*m_pBackBuffer; }
	inline Bind::RenderTarget* GetRenderTarget() const noexcept { return &*m_pRenderTarget; }
	inline Bind::RenderTarget* GetRenderTargetPP() const noexcept { return &*m_pRenderTargetPP; }

	void AddToEvent() noexcept;
	void HandleEvent( Event* event ) override;

private:
	void InitializeDirectX( HWND hWnd, bool resizingWindow );
	bool InitializeShaders();
	bool InitializeRTT();

	// Window data
	Quad m_quad;
	UINT m_viewWidth;
	UINT m_viewHeight;
	float m_clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
	
	ConstantBuffer<RenderToTexture> m_cbPostProcessing;
	float m_overlayColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

	// Shaders
	VertexShader m_vertexShader2D;
	PixelShader m_pixelShader2D;

	VertexShader m_vertexShaderPP;
	PixelShader m_pixelShaderPP;

	// Pipeline components
	std::shared_ptr<Bind::SwapChain> m_pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pContext;

	std::shared_ptr<Bind::DepthStencil> m_pDepthStencil;
	std::shared_ptr<Bind::RenderTarget> m_pRenderTarget;
	std::shared_ptr<Bind::RenderTarget> m_pRenderTargetPP;

	std::shared_ptr<Bind::Viewport> m_pViewport;
	std::shared_ptr<Bind::BackBuffer> m_pBackBuffer;
	std::unordered_map<Bind::Sampler::Type, std::shared_ptr<Bind::Sampler>> m_pSamplerStates;
	std::unordered_map<Bind::Rasterizer::Type, std::shared_ptr<Bind::Rasterizer>> m_pRasterizerStates;
};

#endif