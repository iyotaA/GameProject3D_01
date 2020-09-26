// �萔�o�b�t�@
cbuffer ConstantBuffer:register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
	matrix MtxWIT;
	float4 CameraPosition;
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



// �e�N�X�`��
Texture2D Texture : register(t0);

// �T���v���[
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

	// �n�[�t�����o�[�g
	float light = (dot(-lightDir, input.nomalW) + 1.0f) * 0.5f;
	light = saturate(light);

	// �X�؃L����
	float3 refv = reflect(lightDir, input.nomalW);
	float3 eyev = CameraPosition.xyz - input.posW.xyz;
	refv = normalize(refv);	// ���K��
	eyev = normalize(eyev);	// ���K��

	// ���̔��˂ƃJ�����̎����x�N�g���̓���
	// �i���ˌ����J�����ɓ͂��Ă��邩�H�j
	float  s = dot(refv, eyev);
	s = saturate(s);
	s = pow(s, 10);	// ���E�ڂ���
	float4 speqular = float4(s, s, s, 1.0f);

	float4 outDiffuse = input.color * Material.Diffuse * light * Light.Diffuse;
	outDiffuse += input.color * Material.Ambient * Light.Ambient;
	outDiffuse = float4(Texture.Sample(Sampler, input.uv).rgb * light + speqular.rgb, 1.0f) * outDiffuse;

	return outDiffuse;
}