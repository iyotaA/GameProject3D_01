
// �}�g���N�X�o�b�t�@
cbuffer MatrixBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
    float4 CameraPosition;
}

Texture2D		Texture : register(t0);
SamplerState	Sampler : register(s0);

struct InputData
{
	float4 position : POSITION0;
	float4 normal   : NORMAL0;
	float2 texCoord : TEXCOORD0;
	float4 diffuse  : COLOR;
};

//=============================================================================
// �s�N�Z���V�F�[�_
//=============================================================================
float4 main(in InputData input) : SV_Target
{
	float4 outDiffuse;

	outDiffuse = Texture.Sample(Sampler, input.texCoord);
	outDiffuse *= input.diffuse;

	return outDiffuse;
}
