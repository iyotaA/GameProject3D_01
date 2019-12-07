
//*****************************************************************************
// 定数バッファ
//*****************************************************************************
cbuffer CameraPositionBuffer : register(b0)
{
	float4	CameraPosition;
}

//*****************************************************************************
// グローバル変数
//*****************************************************************************
Texture2D		Texture[2] : register( t0 );

SamplerState	Sampler : register( s0 );


//インプット
struct InputData
{
	float4 position	  : POSITION0;
	float2 uv		  : TEXCOORD0;
	float4 diffuse    : COLOR0;
	float  blendNum   : BLENDNUM0;
};



//=============================================================================
// ピクセルシェーダ
//=============================================================================
void main( in InputData vi, out float4 outDiffuse : SV_Target )
{
	vi.blendNum = saturate(vi.blendNum);

	float4 color1 = Texture[0].Sample(Sampler, vi.uv) * vi.blendNum;
	float4 color2 = Texture[1].Sample(Sampler, vi.uv) * (1.0f - vi.blendNum);

    outDiffuse = color1 + color2;
	outDiffuse *= vi.diffuse;

	outDiffuse.a = 1.0f;
	return;

	// カメラからの距離で霞ませる
	float3 d = distance( CameraPosition.xyz, vi.position.xyz);
	float leng = length(d);
	outDiffuse.a = saturate(1.0f - leng * 0.0005f);
}
