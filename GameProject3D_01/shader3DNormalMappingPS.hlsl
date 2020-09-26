// �萔�o�b�t�@
// 32�r�b�g�Â��ᖳ���ƃo�O��
cbuffer ConstantBuffer:register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
	matrix MtxWIT;
	float4 CameraPosition;
	float  SpequlerPow;
	float3 Dummy;
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

// �e�N�X�`��
// 0 : color1
// 1 : color2
// 2 : normalMap
Texture2D gTextures[3] : register(t0);

// �T���v���[
SamplerState gSampler : register(s0);


////////////////////////////////////////////////
// ���͍\����
////////////////////////////////////////////////
struct InputData
{
	float4 position	 : POSITION0;
	float3 tangentW  : TANGENT0;
	float3 binormalW : BINORMAL0;
	float3 normalW	 : NORMAL0;
	float2 uv		 : TEXCOORD0;
	float3 positionW : POSITION1;
	float  blendNum  : BLENDNUM0;
	float4 color     : COLOR0;
};


float4 main(in InputData input) : SV_TARGET
{
	// �u�����h�l�␳
	input.blendNum = saturate(input.blendNum);

	// �m�[�}���}�b�v( (0.0f �` 1.0f) �� (-1.0f �` 1.0f)  )
	float4 normalMap = gTextures[2].Sample(gSampler, input.uv * 0.99f) * 2.0f - 1.0f;

	// �{�̖@���x�N�g���̕ϊ�
	// tangent��x.y.z�̂ǂꂩ�Ƀ}�C�i�X�����Ȃ��Ƃ��܂������Ȃ�����...
	float3 normalW =
		-normalMap.x * input.binormalW +
		-normalMap.y * input.tangentW +
		-normalMap.z * input.normalW;

	// �m�[�}���C�Y
	normalW = normalize(normalW).xyz;

	float3 lightDir = { 1.0f, -1.0f, -1.0f };
	lightDir = normalize(lightDir).xyz;

	//// �n�[�t�����o�[�g
	float light = (dot(-lightDir, normalW) + 1.0f) * 0.5f;
	light = saturate(light);

	// �X�؃L����
	float3 refv = reflect(lightDir, normalW);
	float3 eyev = CameraPosition.xyz - input.positionW.xyz;
	refv = normalize(refv);	// ���K��
	eyev = normalize(eyev);	// ���K��

	// �X�y�L�����[
	float  s = pow(saturate(dot(refv, eyev)),10);
	float4 speqular = float4(float3(s, s, s) * SpequlerPow, 1.0f);

	// �o�͐F1
	speqular *= input.blendNum;
	float4 color0 = gTextures[0].Sample(gSampler, input.uv) * input.blendNum;
	color0 = color0 * light + speqular;

	// �o�͐F2
	float4 color1 = gTextures[1].Sample(gSampler, input.uv) * (1.0f - input.blendNum);

	float4 outDiffuse = color0 + color1;
	outDiffuse *= input.color;

	// �t�H�O
    float4 fogColor = float4(0.3f, 0.3f, 0.3f, 1.0f);
    float vLength = length(input.positionW.xyz - CameraPosition.xyz);
    float fogFactor = saturate((50.0f - 1.0f) / vLength);
    outDiffuse = fogFactor * outDiffuse + (1.0 - fogFactor) * fogColor;

	return outDiffuse;
}