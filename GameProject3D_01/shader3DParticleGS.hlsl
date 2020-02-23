// =========================================================================
// GPUﾊﾟﾃｲｸﾙシステム
// by: JIWANGGA DIWANGKARA, 2019/12/01
// GPL [https://www.gnu.org/licenses/gpl-3.0.en.html]
// =========================================================================
cbuffer WorldBuffer : register(b0)
{
	matrix World;
}
cbuffer ViewBuffer : register(b1)
{
	matrix View;
}
cbuffer ProjectionBuffer : register(b2)
{
	matrix Projection;
}
cbuffer CameraBuffer : register(b8) //自分の好きな番号に変えた方がいい
{
	float4 CameraPosition;
}

struct vertexInputType
{
	float4 position : POSITION0;
};
struct pixelInputType
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};
struct geometryInputType
{
	float4 position : SV_POSITION;
};
[maxvertexcount(4)]
void main(point geometryInputType Input[1], inout TriangleStream <pixelInputType> TriStream)
{
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);

	float3 wPos = mul(Input[0].position, World);
	float3 planeNormal = wPos - CameraPosition;
	planeNormal.y = 0.0f;
	planeNormal = normalize(planeNormal);

	float3 upVector = float3(0.0f, 2.0f, 0.0f);
	float3 rightVector = normalize(cross(planeNormal, upVector));
	float3 vert[4];
	vert[0] = Input[0].position - rightVector; 
	vert[1] = Input[0].position + rightVector;
	vert[2] = Input[0].position - rightVector + upVector; 
	vert[3] = Input[0].position + rightVector + upVector; 

	float2 texCoord[4];
	texCoord[0] = float2(0, 1);
	texCoord[1] = float2(1, 1);
	texCoord[2] = float2(0, 0);
	texCoord[3] = float2(1, 0);

	pixelInputType p = (pixelInputType)0;
	for (int i = 0; i < 4; i++)
	{
		p.position = mul(float4(vert[i], 1.0f), wvp);
		p.texCoord = texCoord[i];
		TriStream.Append(p);
	}


}
