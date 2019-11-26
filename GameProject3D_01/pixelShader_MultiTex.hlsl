

//*****************************************************************************
// �萔�o�b�t�@
//*****************************************************************************

// �}�e���A���o�b�t�@
cbuffer MaterialBuffer : register( b1 )
{
	float4		Ambient;
	float4		Diffuse;
	float4		Specular;
	float4		Emission;
	float		Shininess;
	float3		Dummy;//16bit���E�p
}





//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
Texture2D		Texture[2] : register( t0 );
SamplerState	Sampler : register( s0 );


//=============================================================================
// �s�N�Z���V�F�[�_
//=============================================================================
void main( in  float4 inPosition		: POSITION0,
						 in  float4 inNormal		: NORMAL0,
						 in  float2 inTexCoord		: TEXCOORD0,
						 in  float4 inDiffuse		: COLOR0,
						 in  float  inBlendNum		: BLENDNUM0,

						 out float4 outDiffuse		: SV_Target )
{

    outDiffuse = Texture[0].Sample(Sampler, inTexCoord) * inBlendNum + Texture[1].Sample(Sampler, inTexCoord) * (1.0f - inBlendNum);

	outDiffuse *= inDiffuse;

}
