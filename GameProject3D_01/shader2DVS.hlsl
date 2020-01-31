

//*****************************************************************************
// �萔�o�b�t�@
//*****************************************************************************

// �}�g���N�X�o�b�t�@
cbuffer MatrixBuffer : register(b0)
{
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



struct InputData
{
	float3 Position	  : POSITION0;
	float4 Diffuse    : COLOR0;
	float2 TexCoord   : TEXCOORD0;
};

struct OutputData
{
    float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD0;
	float4 Diffuse  : COLOR0;
};


//=============================================================================
// ���_�V�F�[�_
//=============================================================================
void main(in  InputData input,	out OutputData output)
{

    output.Diffuse = input.Diffuse;
    output.Position = mul(float4(input.Position, 1.0f), Projection);
    output.TexCoord = input.TexCoord;
}
