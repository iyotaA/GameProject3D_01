#ifndef SHADER_WATER_H_
#define SHADER_WATER_H_

struct VERTEX_3D_WATER
{
	XMFLOAT3 Position;
	XMFLOAT3 Tangent;
	XMFLOAT3 Binormal;
	XMFLOAT3 Normal;
	XMFLOAT4 Diffuse;
	XMFLOAT2 TexCoord;
};


class CShaderWater : public CShaderBase
{
public:
	CShaderWater() {}
	~CShaderWater() {}

	void Init(const char* _VertexShader, const char* _PixelShader);
	void Uninit();
	void Set() {}
	void SetSH();

	void SetMaterial(MATERIAL Material);
	void SetLight(LIGHT Light);

	void SetWorldMatrix(XMFLOAT4X4* _WorldMatrix) { m_Constant.WorldMatrix = Transpose(_WorldMatrix); }
	void SetViewMatrix(XMFLOAT4X4* _ViewMatrix) { m_Constant.ViewMatrix = Transpose(_ViewMatrix); }
	void SetProjectionMatrix(XMFLOAT4X4* ProjectionMatrix) { m_Constant.ProjectionMatrix = Transpose(ProjectionMatrix); }
	void SetMtxWIT(XMFLOAT4X4* _MtxWIT) { m_Constant.MtxWIT = Transpose(_MtxWIT); }
	void SetCameraPosition(XMFLOAT4* _CameraPos) { m_Constant.CameraPosition = *_CameraPos; }
	void SetIncrementValue(float _value) { m_Constant.IncrementValue = _value; }

private:
	struct CONSTANT
	{
		XMFLOAT4X4 WorldMatrix;
		XMFLOAT4X4 ViewMatrix;
		XMFLOAT4X4 ProjectionMatrix;
		XMFLOAT4X4 MtxWIT;
		XMFLOAT4   CameraPosition;
		float      IncrementValue;
	};

	CONSTANT	m_Constant;

};

#endif // !SHADER_WATER_H_

