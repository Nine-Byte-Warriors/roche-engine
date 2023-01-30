#pragma once
#ifndef SAMPLER_H
#define SAMPLER_H

#include <string>
#include <d3d11.h>
#include <Windows.h>
#include <wrl/client.h>
#include "ErrorLogger.h"

namespace Bind
{
	class Sampler
	{
	public:
		enum class Type
		{
			ANISOTROPIC_WRAP,
			ANISOTROPIC_CLAMP,
			BILINEAR,
			POINT
		};
		Sampler( ID3D11Device* device, Type type, bool clamp = false, UINT slot = 0u ) : type( type ), clamp( clamp ), slot( slot )
		{
			try
			{
				CD3D11_SAMPLER_DESC samplerDesc( CD3D11_DEFAULT{} );
				samplerDesc.Filter = [type]() mutable
				{
					switch ( type )
					{
					case Type::ANISOTROPIC_WRAP: case Type::ANISOTROPIC_CLAMP: return D3D11_FILTER_ANISOTROPIC;
					case Type::POINT: return D3D11_FILTER_MIN_MAG_MIP_POINT;
					default:
					case Type::BILINEAR: return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
					}
				}();
				samplerDesc.AddressU = clamp ? D3D11_TEXTURE_ADDRESS_CLAMP : D3D11_TEXTURE_ADDRESS_WRAP;
				samplerDesc.AddressV = clamp ? D3D11_TEXTURE_ADDRESS_CLAMP : D3D11_TEXTURE_ADDRESS_WRAP;
				samplerDesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;

				HRESULT hr = device->CreateSamplerState( &samplerDesc, pSampler.GetAddressOf() );
				COM_ERROR_IF_FAILED( hr, "Failed to create sampler state!" );
			}
			catch ( COMException& exception )
			{
				ErrorLogger::Log( exception );
				return;
			}
		}
		inline void Bind( ID3D11DeviceContext* context ) noexcept
		{
			context->PSSetSamplers( slot, 1u, pSampler.GetAddressOf() );
		}
	private:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
		Type type;
		bool clamp;
		UINT slot;
	};
}

#endif