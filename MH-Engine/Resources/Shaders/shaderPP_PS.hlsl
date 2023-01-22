// Resources
Texture2D textureQuad : register( t0 );
SamplerState samplerState : register( s0 );

// Constant Buffers
cbuffer RenderToTexture : register( b0 )
{
    float3 OverlayColor;
    float Padding;
}

// Pixel Shader
struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD;
};

float4 PS( PS_INPUT input ) : SV_TARGET
{
    // Sample from render target texture
    float3 textureColor = textureQuad.Sample( samplerState, input.TexCoord ).rgb;
    return float4( saturate( textureColor * OverlayColor ), 1.0f );
}