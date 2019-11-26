

//*****************************************************************************
// 定数バッファ
//*****************************************************************************

// マトリクスバッファ
cbuffer WorldBuffer : register( b0 )
{
	matrix World;
}
cbuffer ViewBuffer : register( b1 )
{
	matrix View;
}
cbuffer ProjectionBuffer : register( b2 )
{
	matrix Projection;
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

cbuffer MaterialBuffer : register( b3 )
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

cbuffer LightBuffer : register( b4 )
{
	LIGHT		Light;
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
void main(
	in  InputData inData,
	out OutputData outData
)
{

	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);

	outData.Position = mul(inData.Position, wvp);
	outData.Normal   = inData.Normal;
	outData.TexCoord = inData.TexCoord;

	float4 worldNormal, normal;
	normal = float4(inData.Normal.xyz, 0.0);
	worldNormal = mul(normal, World);
	worldNormal = normalize(worldNormal);

	float light = 0.5 - 0.5 * dot(Light.Direction.xyz, worldNormal.xyz);

	outData.Diffuse = inData.Diffuse * Material.Diffuse * light * Light.Diffuse;
	outData.Diffuse += inData.Diffuse * Material.Ambient * Light.Ambient;
	outData.Diffuse.a = inData.Diffuse.a * Material.Diffuse.a;
}
