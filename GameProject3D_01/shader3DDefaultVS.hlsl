

//*****************************************************************************
// 定数バッファ
//*****************************************************************************

// マトリクスバッファ
cbuffer MatrixBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
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



struct InputData
{
	float4 Position	  : POSITION0;
	float4 Normal     : NORMAL0;
	float4 Diffuse    : COLOR0;
	float2 TexCoord   : TEXCOORD0;
};

struct OutputData
{
	float4 Position : SV_POSITION;
	float4 Normal   : NORMAL0;
	float2 TexCoord : TEXCOORD0;
	float4 Diffuse  : COLOR0;
};


//=============================================================================
// 頂点シェーダ
//=============================================================================
void main(in  InputData input,	out OutputData output)
{

	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);

	output.Position = mul(input.Position, wvp);
	output.Normal = input.Normal;
	output.TexCoord = input.TexCoord;

	float4 worldNormal, normal;
	normal = float4(input.Normal.xyz, 0.0);
	worldNormal = mul(normal, World);
	worldNormal = normalize(worldNormal);

	float light = 0.5 - 0.5 * dot(Light.Direction.xyz, worldNormal.xyz);

	output.Diffuse = input.Diffuse * Material.Diffuse * light * Light.Diffuse;
	output.Diffuse += input.Diffuse * Material.Ambient * Light.Ambient;
	output.Diffuse.a = input.Diffuse.a * Material.Diffuse.a;
}
