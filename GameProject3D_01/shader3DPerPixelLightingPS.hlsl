// 定数バッファ
cbuffer ConstantBuffer:register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
	matrix MtxWIT;
	float4 CameraPosition;
}

// ライトバッファ
struct LIGHT
{
	float4		Direction;
	float4		Diffuse;
	float4		Ambient;
};
cbuffer LightBuffer : register(b1)
{
	LIGHT		Light;
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
cbuffer MaterialBuffer : register(b2)
{
	MATERIAL	Material;
}



// テクスチャ
Texture2D Texture : register(t0);

// サンプラー
SamplerState Sampler : register(s0);



struct InputPS
{
	float4 pos		: POSITION0;
	float2 uv		: TEXCOORD0;
	float3 posW		: POSITION1;
	float3 nomalW	: NORMAL0;
	float4 color	: COLOR0;
};


float4 main(InputPS input) : SV_TARGET
{
	float3 lightDir = {1.0f, -1.0f, 1.0f};
	lightDir = normalize(lightDir);

	// ハーフランバート
	float light = (dot(-lightDir, input.nomalW) + 1.0f) * 0.5f;
	light = saturate(light);

	// スぺキュラ
	float3 refv = reflect(lightDir, input.nomalW);
	float3 eyev = CameraPosition.xyz - input.posW.xyz;
	refv = normalize(refv);	// 正規化
	eyev = normalize(eyev);	// 正規化

	// 光の反射とカメラの視線ベクトルの内積
	// （反射光がカメラに届いているか？）
	float  s = dot(refv, eyev);
	s = saturate(s);
	s = pow(s, 10);	// 境界ぼかし
	float4 speqular = float4(s, s, s, 1.0f);

	float4 outDiffuse = input.color * Material.Diffuse * light * Light.Diffuse;
	outDiffuse += input.color * Material.Ambient * Light.Ambient;
	outDiffuse = float4(Texture.Sample(Sampler, input.uv).rgb * light + speqular.rgb, 1.0f) * outDiffuse;

	return outDiffuse;
}