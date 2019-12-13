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
	float4 pos   : POSITION0;
	float4 nomal : NORMAL0;
	float4 color : COLOR0;
	float2 uv    : TEXCOORD0;
};

struct OutputData
{
	float4 pos		: SV_POSITION;
	float2 uv		: TEXCOORD0;
	float3 posW		: POSITION1;
	float3 nomalW	: NORMAL0;
	float4 color	: COLOR0;
};


///////////////////////////////////////////////
// メイン関数
///////////////////////////////////////////////
void main(in InputData input, out OutputData output)
{
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);
	output.pos = mul(input.pos, wvp);

	input.nomal.w = 0.0f;
	output.nomalW = mul(input.nomal, MtxWIT).xyz;

	// ワールド座標取得
	output.posW = mul(input.pos, World);

	// UV
	output.uv = input.uv;

	// color
	output.color = input.color;

	return;
}
