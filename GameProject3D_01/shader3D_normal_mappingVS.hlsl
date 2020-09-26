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
};

struct OutputData
{
	float4 position	 : SV_POSITION;
	float3 tangentW  : TANGENT0;
	float3 binormalW : BINORMAL0;
	float3 nomalW	 : NORMAL0;
	float2 uv		 : TEXCOORD0;
	float3 posW		 : POSITION1;
};


///////////////////////////////////////////////
// ���C���֐�
///////////////////////////////////////////////


void main( in InputData InputVS, out OutputData OutputVS )
{
	// W * V * P �ϊ���̃|�W�V����
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);
	OutputVS.position = mul(InputVS.position, wvp);

	// �@��
	InputVS.nomal.w = 0.0f;
	OutputVS.nomalW = mul(InputVS.nomal, MtxWIT).xyz;

	// �^���W�F���g
	InputVS.tangent.w = 0.0f;
	OutputVS.tangentW = mul(InputVS.tangent, MtxWIT).xyz;

	// �o�C�m�[�}��
	InputVS.binormal.w = 0.0f;
	OutputVS.binormalW = mul(InputVS.binormal, MtxWIT).xyz;

	// ���[���h���W�擾
	OutputVS.posW = mul(InputVS.position, World).xyz;

	// UV
	OutputVS.uv = InputVS.uv;
}