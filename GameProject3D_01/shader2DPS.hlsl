
//*****************************************************************************
// レジスタ
//*****************************************************************************
Texture2D		Texture : register(t0);
SamplerState	Sampler : register(s0);

struct InputData
{
    float4 Position : POSITION0;
	float2 TexCoord : TEXCOORD0;
	float4 Diffuse  : COLOR0;
};

//=============================================================================
// ピクセルシェーダ
//=============================================================================
float4 main(in InputData input) : SV_Target
{
    return Texture.Sample(Sampler, input.TexCoord) * input.Diffuse;
}
