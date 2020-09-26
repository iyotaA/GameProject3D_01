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
};

struct OutputData
{
	float4 position	 : SV_POSITION;
	float3 tangentW  : TANGENT0;
	float3 binormalW : BINORMAL0;
	float3 nomalW	 : NORMAL0;
	float2 uv		 : TEXCOORD0;
	float3 posW		 : POSITION1;
};


///////////////////////////////////////////////
// メイン関数
///////////////////////////////////////////////


void main( in InputData InputVS, out OutputData OutputVS )
{
	// W * V * P 変換後のポジション
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);
	OutputVS.position = mul(InputVS.position, wvp);

	// 法線
	InputVS.nomal.w = 0.0f;
	OutputVS.nomalW = mul(InputVS.nomal, MtxWIT).xyz;

	// タンジェント
	InputVS.tangent.w = 0.0f;
	OutputVS.tangentW = mul(InputVS.tangent, MtxWIT).xyz;

	// バイノーマル
	InputVS.binormal.w = 0.0f;
	OutputVS.binormalW = mul(InputVS.binormal, MtxWIT).xyz;

	// ワールド座標取得
	OutputVS.posW = mul(InputVS.position, World).xyz;

	// UV
	OutputVS.uv = InputVS.uv;
}