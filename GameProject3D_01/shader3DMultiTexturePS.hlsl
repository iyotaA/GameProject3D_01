
//*****************************************************************************
// �萔�o�b�t�@
//*****************************************************************************
cbuffer CameraPositionBuffer : register(b0)
{
	float4	CameraPosition;
}

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
Texture2D		Texture[2] : register( t0 );

SamplerState	Sampler : register( s0 );


//�C���v�b�g
struct InputData
{
	float4 position	  : POSITION0;
	float2 uv		  : TEXCOORD0;
	float4 diffuse    : COLOR0;
	float  blendNum   : BLENDNUM0;
};



//=============================================================================
// �s�N�Z���V�F�[�_
//=============================================================================
float4 main( in InputData input ) : SV_Target
{
	input.blendNum = saturate(input.blendNum);

	float4 color1 = Texture[0].Sample(Sampler, input.uv) * input.blendNum;
	float4 color2 = Texture[1].Sample(Sampler, input.uv) * (1.0f - input.blendNum);

	float4 outDiffuse;

    outDiffuse = color1 + color2;
	outDiffuse *= input.diffuse;

	outDiffuse.a = 1.0f;
	return outDiffuse;

	// �J��������̋����ŉ��܂���
	float3 d = CameraPosition.xyz - input.position.xyz;
	float leng = length(d);
	outDiffuse.a = saturate(1.0f - leng * 0.0002f);

	return outDiffuse;
}
