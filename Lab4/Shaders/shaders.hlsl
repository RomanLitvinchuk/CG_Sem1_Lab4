cbuffer ConstantBuffer : register(b0)
{
    float4x4 mWorldViewProj;
}

void VS(float3 iPosL : POSITION,
        float4 iColor : COLOR,
        out float4 oPosH : SV_POSITON,
        out float4 oColor : COLOR)
{
    oPosH = mul(float4(iPosL, 1.0f), mWorldViewProj);
    oColor = iColor;
}