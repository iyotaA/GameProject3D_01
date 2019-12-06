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
cbuffer LightBuffer : register(b2)
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
cbuffer MaterialBuffer : register(b1)
{
	MATERIAL	Material;
}



// �e�N�X�`��
Texture2D Texture : register(t0);

// �T���v���[
SamplerState Sampler : register(s0);



struct PS_IN
{
	float4 pos		: POSITION0;
	float2 uv		: TEXCOORD0;
	float3 posW		: POSITION1;
	float3 nomalW	: NORMAL0;
};


float4 main(PS_IN pin) : SV_TARGET
{
	float3 lightDir = {1.0f, -1.0f, 1.0f};
	lightDir = normalize(lightDir);

	// �n�[�t�����o�[�g
	float light = (dot(-lightDir, pin.nomalW) + 1.0f) * 0.5f;
	light = saturate(light);

	// �X�؃L����
	float3 refv = reflect(lightDir, pin.nomalW);
	float3 eyev = CameraPosition.xyz - pin.posW.xyz;
	refv = normalize(refv);	// ���K��
	eyev = normalize(eyev);	// ���K��

	// ���̔��˂ƃJ�����̎����x�N�g���̓���
	// �i���ˌ����J�����ɓ͂��Ă��邩�H�j
	float  s = dot(refv, eyev);
	s = saturate(s);
	s = pow(s, 10);	// ���E�ڂ���
	float4 speqular = float4(s, s, s, 1.0f);


	return float4(Texture.Sample(Sampler, pin.uv).rgb * light + speqular.rgb, 1.0f);
}