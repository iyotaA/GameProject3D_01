// 定数バッファ
cbuffer ConstantBuffer:register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
	matrix MtxWIT;
	float4 CameraPosition;
	float  IncrementValue;
}

// マテリアルバッファ
struct MATERIAL
{
	float4		Ambient;
	float4		Diffuse;
	float4		Specular;
	float4		Emission;
	float		Shininess;
	float3		Dummy;//16bit境界用
};
cbuffer MaterialBuffer : register(b1)
{
	MATERIAL	Material;
}

// ライトバッファ
struct LIGHT
{
	float4		Direction;
	float4		Diffuse;
	float4		Ambient;
};
cbuffer LightBuffer : register(b2)
{
	LIGHT		Light;
}

// テクスチャ
// 0 : color1
// 1 : normalMap
Texture2D gTextures[2] : register(t0);

// サンプラー
SamplerState gSampler : register(s0);


////////////////////////////////////////////////
// 入力構造体
////////////////////////////////////////////////
struct InputData
{
	float4 position	 : POSITION0;
	float3 tangentW  : TANGENT0;
	float3 binormalW : BINORMAL0;
	float3 normalW	 : NORMAL0;
	float2 uv		 : TEXCOORD0;
	float3 positionW : POSITION1;
	float4 color     : COLOR0;
};


float4 main(in InputData input) : SV_TARGET
{
	// ノーマルマップ( (0.0f 〜 1.0f) → (-1.0f 〜 1.0f)  )
	float4 normalMap = gTextures[1].Sample(gSampler, input.uv) * 2.0f - 1.0f;

	// 本の法線ベクトルの変換
	// tangentのx.y.zのどれかにマイナスを入れないとうまくいかないかも...
	float3 normalW =
		-normalMap.x * input.binormalW +
		-normalMap.y * input.tangentW +
		-normalMap.z * input.normalW;

	// ノーマライズ
	normalW = normalize(normalW).xyz;

	float3 lightDir = { 1.0f, -1.0f, 1.0f };
	lightDir = normalize(lightDir).xyz;

	//// ハーフランバート
	float light = (dot(-lightDir, normalW) + 1.0f) * 0.5f;
	light = saturate(light);

	// スぺキュラ
	float3 refv = reflect(lightDir, normalW);
	float3 eyev = CameraPosition.xyz - input.positionW.xyz;
	refv = normalize(refv);	// 正規化
	eyev = normalize(eyev);	// 正規化

	// スペキュラー
	float  s = pow(saturate(dot(refv, eyev)), 10);
	float4 speqular = float4(float3(s, s, s) * 0.2f, 1.0f);

	// 出力色
	float4 outDiffuse = gTextures[0].Sample(gSampler, input.uv);
	outDiffuse *= input.color;
	return outDiffuse;
}