// �萔�o�b�t�@
cbuffer ConstantBuffer:register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
	matrix MtxWIT;
	float4 CameraPosition;
}

struct InputData
{
	float4 position : POSITION0;
	float4 tangent  : TANGENT0;
	float4 binormal : BINORMAL0;
	float4 nomal    : NORMAL0;
	float4 color    : COLOR0;
	float2 uv       : TEXCOORD0;
	float  blendNum : BLENDNUM0;
};

struct OutputData
{
	float4 position	 : SV_POSITION;
	float3 tangentW  : TANGENT0;
	float3 binormalW : BINORMAL0;
	float3 nomalW	 : NORMAL0;
	float2 uv		 : TEXCOORD0;
	float3 positionW : POSITION1;
	float  blendNum  : BLENDNUM0;
	float4 color     : COLOR0;
};


///////////////////////////////////////////////
// ���C���֐�
///////////////////////////////////////////////


void main( in InputData input, out OutputData output )
{
	// W * V * P �ϊ���̃|�W�V����
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);
	output.position = mul(input.position, wvp);

	// �@��
	input.nomal.w = 0.0f;
	output.nomalW = mul(input.nomal, MtxWIT).xyz;

	// �^���W�F���g
	input.tangent.w = 0.0f;
	output.tangentW = mul(input.tangent, MtxWIT).xyz;

	// �o�C�m�[�}��
	input.binormal.w = 0.0f;
	output.binormalW = mul(input.binormal, MtxWIT).xyz;

	// ���[���h���W�擾
	output.positionW = mul(input.position, World).xyz;

	// UV
	output.uv = input.uv;

	// �u�����h�l
	output.blendNum = input.blendNum;

	// ���_�F
	output.color = input.color;
}