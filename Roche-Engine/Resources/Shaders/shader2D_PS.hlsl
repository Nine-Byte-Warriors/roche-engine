// Resources
Texture2D objTexture : TEXTURE : register( t0 );
SamplerState samplerState : SAMPLER : register( s0 );

// Pixel Shader
struct PS_INPUT
{
	float4 inPosition : SV_POSITION;
    float2 inTexCoord : TEXCOORD;
};

float4 PS( PS_INPUT input ) : SV_TARGET
{
    float4 finalColor = objTexture.Sample( samplerState, input.inTexCoord );
    return float4( saturate( (float3)finalColor ), 1.0f );
}