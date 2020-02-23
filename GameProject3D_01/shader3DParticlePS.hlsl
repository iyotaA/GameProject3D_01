

//=============================================================================
// グローバル変数
//=============================================================================
Texture2D		g_Texture : register( t0 );
SamplerState	g_SamplerState : register( s0 );

struct pixelInputType
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD0;

};

//=============================================================================
// ピクセルシェーダ
//=============================================================================
float4 main(pixelInputType input) : SV_Target
{
	float4 output = float4(1,1,1,1);

	output = g_Texture.Sample(g_SamplerState, input.texcoord);
	clip(output.a - 0.25f);

	return output;
}
