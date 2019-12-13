
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

//インプット
struct InputData
{
	float4 position	  : POSITION0;
	float4 normal     : NORMAL0;
	float4 diffuse    : COLOR0;
	float2 uv		  : TEXCOORD0;
	float  blendNum   : BLENDNUM0;
};

// アウトプット
struct OutputData
{
	float4 position	: SV_POSITION;
	float2 uv		: TEXCOORD0;
	float4 diffuse  : COLOR0;
	float  blendNum : BLENDNUM0;
};


//=============================================================================
// 頂点シェーダ
//=============================================================================
void main( in  InputData input, out OutputData output)
{
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);

	output.position = mul(input.position, wvp);
	output.uv        = input.uv;

	float4 worldNormal, normal;
	normal      = float4(input.normal.xyz, 0.0);
	worldNormal = mul(normal, World);
	worldNormal = normalize(worldNormal);

	float light = 0.5 - 0.5 * dot(Light.Direction.xyz, worldNormal.xyz);

	output.diffuse   = input.diffuse * Material.Diffuse * light * Light.Diffuse;
	output.diffuse  += input.diffuse * Material.Ambient * Light.Ambient;
	output.diffuse.a = input.diffuse.a * Material.Diffuse.a;
	output.blendNum  = input.blendNum;
}
