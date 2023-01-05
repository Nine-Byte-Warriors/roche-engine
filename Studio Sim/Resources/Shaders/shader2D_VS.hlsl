// Constant Buffers
cbuffer ObjectBuffer : register( b0 )
{
    float4x4 wvpMatrix;
}

// Vertex Shader
struct VS_INPUT
{
    float3 inPosition : POSITION;
    float2 inTexCoord : TEXCOORD;
};

struct VS_OUPUT
{
    float4 outPosition : SV_POSITION;
    float2 outTexCoord : TEXCOORD;
};

VS_OUPUT VS( VS_INPUT input )
{
    VS_OUPUT output;
    output.outPosition = mul( float4( input.inPosition, 1.0f ), wvpMatrix );
    output.outTexCoord = input.inTexCoord;
    return output;
}