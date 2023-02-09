#include "stdafx.h"
#include "Graphics.h"

#if _DEBUG
extern bool g_bDebug;
#endif

bool Graphics::Initialize( HWND hWnd, UINT width, UINT height )
{
	m_viewWidth = width;
	m_viewHeight = height;

	AddToEvent();
	InitializeDirectX( hWnd, false );

	if ( !InitializeShaders() )
		return false;

	if ( !InitializeRTT() )
		return false;

	return true;
}

void Graphics::InitializeDirectX( HWND hWnd, bool resizingWindow )
{
	if ( !resizingWindow )
		m_pSwapChain = std::make_shared<Bind::SwapChain>( m_pContext.GetAddressOf(), m_pDevice.GetAddressOf(), hWnd, m_viewWidth, m_viewHeight );

    m_pBackBuffer = std::make_shared<Bind::BackBuffer>( m_pDevice.Get(), m_pSwapChain->GetSwapChain() );
	m_pRenderTarget = std::make_shared<Bind::RenderTarget>( m_pDevice.Get(), m_viewWidth, m_viewHeight );
	m_pRenderTargetPP = std::make_shared<Bind::RenderTarget>( m_pDevice.Get(), m_viewWidth, m_viewHeight );
    m_pDepthStencil = std::make_shared<Bind::DepthStencil>( m_pDevice.Get(), m_viewWidth, m_viewHeight );
	m_pViewport = std::make_shared<Bind::Viewport>( m_pContext.Get(), m_viewWidth, m_viewHeight );

    m_pRasterizerStates.emplace( Bind::Rasterizer::Type::SOLID, std::make_shared<Bind::Rasterizer>( m_pDevice.Get(), Bind::Rasterizer::Type::SOLID ) );
    m_pRasterizerStates.emplace( Bind::Rasterizer::Type::WIREFRAME, std::make_shared<Bind::Rasterizer>( m_pDevice.Get(), Bind::Rasterizer::Type::WIREFRAME ) );

    m_pSamplerStates.emplace( Bind::Sampler::Type::ANISOTROPIC_WRAP, std::make_shared<Bind::Sampler>( m_pDevice.Get(), Bind::Sampler::Type::ANISOTROPIC_WRAP, false, 0u ) );
	m_pSamplerStates.emplace( Bind::Sampler::Type::ANISOTROPIC_CLAMP, std::make_shared<Bind::Sampler>( m_pDevice.Get(), Bind::Sampler::Type::ANISOTROPIC_CLAMP, true, 1u ) );
	m_pSamplerStates.emplace( Bind::Sampler::Type::BILINEAR, std::make_shared<Bind::Sampler>( m_pDevice.Get(), Bind::Sampler::Type::BILINEAR ) );
	m_pSamplerStates.emplace( Bind::Sampler::Type::POINT, std::make_shared<Bind::Sampler>( m_pDevice.Get(), Bind::Sampler::Type::POINT ) );

	m_pSamplerStates[Bind::Sampler::Type::ANISOTROPIC_WRAP]->Bind( m_pContext.Get() );
    m_pContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
}

bool Graphics::InitializeShaders()
{
	try
	{
		// Define input layout for textures
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		// Create the sprite shaders
		HRESULT hr = m_vertexShader2D.Initialize( m_pDevice, L"Resources\\Shaders\\shader2D_VS.hlsl", layout, ARRAYSIZE( layout ) );
		COM_ERROR_IF_FAILED( hr, "Failed to create sprite vertex shader!" );
		hr = m_pixelShader2D.Initialize( m_pDevice, L"Resources\\Shaders\\shader2D_PS_Discard.hlsl" );
		COM_ERROR_IF_FAILED( hr, "Failed to create sprite pixel shader!" );

		// Define input layout for RTT
		D3D11_INPUT_ELEMENT_DESC layoutPP[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		// Create the RTT shaders
		hr = m_vertexShaderPP.Initialize( m_pDevice, L"Resources\\Shaders\\shaderPP_VS.hlsl", layoutPP, ARRAYSIZE( layoutPP ) );
		COM_ERROR_IF_FAILED( hr, "Failed to create RTT vertex shader!" );
		hr = m_pixelShaderPP.Initialize( m_pDevice, L"Resources\\Shaders\\shaderPP_PS.hlsl" );
		COM_ERROR_IF_FAILED( hr, "Failed to create RTT pixel shader!" );
	}
	catch ( COMException& exception )
	{
		ErrorLogger::Log( exception );
		return false;
	}

	return true;
}

void Graphics::UpdateRenderState()
{
	// Set default render state for objects
    m_pRasterizerStates[Bind::Rasterizer::Type::SOLID]->Bind( m_pContext.Get() );
	Shaders::BindShaders( m_pContext.Get(), m_vertexShader2D, m_pixelShader2D );
}

bool Graphics::InitializeRTT()
{
	try
	{
		HRESULT hr = m_cbPostProcessing.Initialize( m_pDevice.Get(), m_pContext.Get() );
		COM_ERROR_IF_FAILED( hr, "Failed to create 'PostProcessing' constant buffer!" );

		if ( !m_quad.Initialize( m_pDevice.Get() ) )
			return false;
	}
	catch ( COMException& exception )
	{
		ErrorLogger::Log( exception );
		return false;
	}
	return true;
}

#if _DEBUG
void Graphics::SpawnControlWindow()
{
	if ( ImGui::Begin( "Post-Processing", FALSE, ImGuiWindowFlags_AlwaysAutoResize ) )
	{
		ImGui::Text( "Color Overlay" );
		ImGui::SliderFloat3( "##ColorOverlay", m_overlayColor, 0.0f, 1.0f, "%.1f" );
	}
	ImGui::End();
}
#endif

void Graphics::RenderSceneToTexture()
{
	// 1. Bind new render target
	m_pRenderTargetPP->Bind( m_pContext.Get(), m_pDepthStencil.get(), m_clearColor );

	XMFLOAT3 overlayColor = { m_overlayColor[0], m_overlayColor[1], m_overlayColor[2] };
	m_cbPostProcessing.data.OverlayColor = overlayColor;
	if (!m_cbPostProcessing.ApplyChanges()) return;

	// 2. Render fullscreen texture to new render target
	Shaders::BindShaders( m_pContext.Get(), m_vertexShaderPP, m_pixelShaderPP );
	m_quad.SetupBuffers( m_pContext.Get() );

	m_pContext->PSSetConstantBuffers( 0u, 1u, m_cbPostProcessing.GetAddressOf() );
	m_pContext->PSSetShaderResources( 0u, 1u, m_pRenderTarget->GetShaderResourceViewPtr() );

	Bind::Rasterizer::DrawSolid( m_pContext.Get(), m_quad.GetIndexBuffer().IndexCount() ); // always draw as solid

	// 3. Bind back buffer only for rendering imgui windows
	m_pBackBuffer->Bind( m_pContext.Get(), m_pDepthStencil.get(), m_clearColor );

#if !_DEBUG
	// 4. Render only the scene to the back buffer
	Shaders::BindShaders( m_pContext.Get(), m_vertexShaderPP, m_pixelShaderPP );
	m_quad.SetupBuffers( m_pContext.Get() );

	m_pContext->PSSetConstantBuffers( 0u, 1u, m_cbPostProcessing.GetAddressOf() );
	m_pContext->PSSetShaderResources( 0u, 1u, m_pRenderTarget->GetShaderResourceViewPtr() );

	Bind::Rasterizer::DrawSolid( m_pContext.Get(), m_quad.GetIndexBuffer().IndexCount() ); // always draw as solid
#else
	if ( !g_bDebug )
	{
		// 4. Render only the scene to the back buffer
		Shaders::BindShaders( m_pContext.Get(), m_vertexShaderPP, m_pixelShaderPP );
		m_quad.SetupBuffers( m_pContext.Get() );

		m_pContext->PSSetConstantBuffers( 0u, 1u, m_cbPostProcessing.GetAddressOf() );
		m_pContext->PSSetShaderResources( 0u, 1u, m_pRenderTarget->GetShaderResourceViewPtr() );

		Bind::Rasterizer::DrawSolid( m_pContext.Get(), m_quad.GetIndexBuffer().IndexCount() ); // always draw as solid
	}
#endif
}

void Graphics::BeginFrame()
{
	// Clear render target/depth stencil
    m_pRenderTarget->Bind( m_pContext.Get(), m_pDepthStencil.get(), m_clearColor );
    m_pDepthStencil->ClearDepthStencil( m_pContext.Get() );
}

void Graphics::EndFrame()
{
	// Unbind render targets
	m_pRenderTargetPP->BindNull( m_pContext.Get() );
	m_pRenderTarget->BindNull( m_pContext.Get() );
	m_pBackBuffer->BindNull( m_pContext.Get() );

	// Present frame
	HRESULT hr = m_pSwapChain->GetSwapChain()->Present( 1u, NULL );
	if ( FAILED( hr ) )
	{
		hr == DXGI_ERROR_DEVICE_REMOVED ?
			ErrorLogger::Log( m_pDevice->GetDeviceRemovedReason(), "Swap Chain. Graphics device removed!" ) :
			ErrorLogger::Log( hr, "Swap Chain failed to render frame!" );
		exit( -1 );
	}
}

void Graphics::AddToEvent() noexcept
{
	EventSystem::Instance()->AddClient( EVENTID::WindowSizeChangeEvent, this );
	EventSystem::Instance()->AddClient( EVENTID::UpdateSettingsEvent, this );
	EventSystem::Instance()->AddClient( EVENTID::RedOverlayColour, this );
	EventSystem::Instance()->AddClient( EVENTID::GreenOverlayColour, this );
	EventSystem::Instance()->AddClient( EVENTID::BlueOverlayColour, this );
}

void Graphics::RemoveFromEvent() noexcept
{
	EventSystem::Instance()->RemoveClient( EVENTID::WindowSizeChangeEvent, this );
	EventSystem::Instance()->RemoveClient( EVENTID::UpdateSettingsEvent, this );
	EventSystem::Instance()->RemoveClient( EVENTID::RedOverlayColour, this );
	EventSystem::Instance()->RemoveClient( EVENTID::GreenOverlayColour, this );
	EventSystem::Instance()->RemoveClient( EVENTID::BlueOverlayColour, this );
}

void Graphics::HandleEvent( Event* event )
{
	switch ( event->GetEventID() )
	{
	case EVENTID::RedOverlayColour: { m_overlayColor[0] = *static_cast<float*>( event->GetData() ); } break;
	case EVENTID::GreenOverlayColour: { m_overlayColor[1] = *static_cast<float*>( event->GetData() ); } break;
	case EVENTID::BlueOverlayColour: { m_overlayColor[2] = *static_cast<float*>( event->GetData() ); } break;
	case EVENTID::WindowSizeChangeEvent:
	{
		XMFLOAT2 sizeOfScreen = *static_cast<XMFLOAT2*>( event->GetData() );
		m_viewWidth = sizeOfScreen.x;
		m_viewHeight = sizeOfScreen.y;

		// Window size: changing buffers size
		m_pContext->OMSetRenderTargets( 0u, nullptr, nullptr );

		// Clear all buffers
		m_pBackBuffer.reset();
		m_pRenderTarget.reset();
		m_pRenderTargetPP.reset();
		m_pRasterizerStates.clear();
		m_pSamplerStates.clear();
		m_pDepthStencil.reset();
		m_pViewport.reset();

		try
		{
			// Preserve the existing buffer count and format.
			// Automatically choose the width and height to match the client rect for HWNDs.
			HRESULT hr = m_pSwapChain->GetSwapChain()->ResizeBuffers( 0u, 0u, 0u, DXGI_FORMAT_UNKNOWN, 0u );
			COM_ERROR_IF_FAILED( hr, "Failed to resize buffers on window resize!" );
			InitializeDirectX( nullptr, true );
		}
		catch ( COMException& exception )
		{
			ErrorLogger::Log( exception );
			return;
		}
	}
	break;
	}
}