#pragma pack_matrix( row_major )

// Constant Buffers
cbuffer ObjectBuffer : register( b0 )
{
    float4x4 wvpMatrix;
}

cbuffer Animation : register( b1 )
{
    float Width;
    float Height;
    float Rows;
    float Columns;
    
    int FrameX;
    int FrameY;
    float2 Padding;
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
    input.inTexCoord.x = input.inTexCoord.x + FrameX;
    input.inTexCoord.x = input.inTexCoord.x / Columns;
    
    input.inTexCoord.y = input.inTexCoord.y + FrameY;
    input.inTexCoord.y = input.inTexCoord.y / Rows;
    
    output.outPosition = mul( float4( input.inPosition, 1.0f ), wvpMatrix );
    output.outTexCoord = input.inTexCoord;
    return output;
}