
//*****************************************************************************
// �萔�o�b�t�@
//*****************************************************************************

// �}�g���N�X�o�b�t�@
cbuffer MatrixBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
}

// ���C�g�o�b�t�@
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

// �}�e���A���o�b�t�@
struct MATERIAL
{
	float4		Ambient;
	float4		Diffuse;
	float4		Specular;
	float4		Emission;
	float		Shininess;
	float3		Dummy;//16bit���E�p
};

cbuffer MaterialBuffer : register(b2)
{
	MATERIAL	Material;
}

//�C���v�b�g
struct InputData
{
	float4 position	  : POSITION0;
	float4 normal     : NORMAL0;
	float4 diffuse    : COLOR0;
	float2 uv		  : TEXCOORD0;
	float  blendNum   : BLENDNUM0;
};

// �A�E�g�v�b�g
struct OutputData
{
	float3 pos		: POSITION0;
	float2 uv		: TEXCOORD0;
	float4 diffuse  : COLOR0;
	float  blendNum : BLENDNUM0;
};


//=============================================================================
// ���_�V�F�[�_
//=============================================================================
void main( in  InputData vi, out OutputData vo, out float4 Position : SV_POSITION )
{
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);

	Position  = mul(vi.position, wvp);
	vo.pos    = mul(vi.position, wvp);
	vo.uv     = vi.uv;

	float4 worldNormal, normal;
	normal      = float4(vi.normal.xyz, 0.0);
	worldNormal = mul(normal, World);
	worldNormal = normalize(worldNormal);

	float light = 0.5 - 0.5 * dot(Light.Direction.xyz, worldNormal.xyz);

	vo.diffuse   = vi.diffuse * Material.Diffuse * light * Light.Diffuse;
	vo.diffuse  += vi.diffuse * Material.Ambient * Light.Ambient;
	vo.diffuse.a = vi.diffuse.a * Material.Diffuse.a;
	vo.blendNum  = vi.blendNum;
}
