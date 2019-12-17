
//*****************************************************************************
// グローバル変数
//*****************************************************************************
Texture2D		g_Texture : register(t0);
SamplerState	g_SamplerState : register(s0);

struct InputData
{
	float4 position : POSITION0;
	float4 normal   : NORMAL0;
	float2 texCoord : TEXCOORD0;
	float4 diffuse  : COLOR;
};

//=============================================================================
// ピクセルシェーダ
//=============================================================================
float4 main(in InputData input) : SV_Target
{
	float4 outDiffuse;

	outDiffuse = g_Texture.Sample(g_SamplerState, input.texCoord);
	outDiffuse *= input.diffuse;

	return outDiffuse;
	//return float4(1.0f, 1.0f, 1.0f, 1.0f);
}
