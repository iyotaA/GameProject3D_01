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
};


///////////////////////////////////////////////
// メイン関数
///////////////////////////////////////////////
void main(in InputData vi, out OutputData vo)
{
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);
	vo.pos = mul(vi.pos, wvp);

	vi.nomal.w = 0.0f;
	vo.nomalW = mul(vi.nomal, MtxWIT).xyz;

	// ワールド座標取得
	vo.posW = mul(vi.pos, World);

	// UV
	vo.uv = vi.uv;

	return;
}
