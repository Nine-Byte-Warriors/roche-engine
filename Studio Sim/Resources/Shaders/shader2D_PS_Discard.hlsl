// Resources
Texture2D objTexture : TEXTURE : register( t0 );
SamplerState samplerState : SAMPLER : register( s0 );

// Pixel Shader
struct PS_INPUT
{
	float4 inPosition : SV_POSITION;
    float2 inTexCoord : TEXCOORD;
};

void PS( PS_INPUT input )
{
    float4 sampleColor = objTexture.Sample( samplerState, input.inTexCoord );
    if ( sampleColor.a < 0.25 )
        discard;
}