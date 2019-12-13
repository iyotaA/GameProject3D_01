// 定数バッファ
cbuffer ConstantBuffer:register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
	matrix MtxWIT;
	float4 CameraPosition;
}

struct InputData
{
	float4 position : POSITION0;
	float4 tangent  : TANGENT0;
	float4 binormal : BINORMAL0;
	float4 nomal    : NORMAL0;
	float4 color    : COLOR0;
	float2 uv       : TEXCOORD0;
	float  blendNum : BLENDNUM0;
};

struct OutputData
{
	float4 position	 : SV_POSITION;
	float3 tangentW  : TANGENT0;
	float3 binormalW : BINORMAL0;
	float3 nomalW	 : NORMAL0;
	float2 uv		 : TEXCOORD0;
	float3 positionW : POSITION1;
	float  blendNum  : BLENDNUM0;
	float4 color     : COLOR0;
};


///////////////////////////////////////////////
// メイン関数
///////////////////////////////////////////////


void main( in InputData input, out OutputData output )
{
	// W * V * P 変換後のポジション
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);
	output.position = mul(input.position, wvp);

	// 法線
	input.nomal.w = 0.0f;
	output.nomalW = mul(input.nomal, MtxWIT).xyz;

	// タンジェント
	input.tangent.w = 0.0f;
	output.tangentW = mul(input.tangent, MtxWIT).xyz;

	// バイノーマル
	input.binormal.w = 0.0f;
	output.binormalW = mul(input.binormal, MtxWIT).xyz;

	// ワールド座標取得
	output.positionW = mul(input.position, World).xyz;

	// UV
	output.uv = input.uv;

	// ブレンド値
	output.blendNum = input.blendNum;

	// 頂点色
	output.color = input.color;
}