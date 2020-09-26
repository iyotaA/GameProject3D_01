#ifndef SHADER_NORMAL_MAPPING_H_
#define SHADER_NORMAL_MAPPING_H_

struct VERTEX_3D_NOMAL_MAP
{
	XMFLOAT3 Position;
	XMFLOAT3 Tangent;
	XMFLOAT3 Binormal;
	XMFLOAT3 Normal;
	XMFLOAT4 Diffuse;
	XMFLOAT2 TexCoord;
	float	 BlendNum;
};

class CShaderNormalMap : public CShaderBase
{
public:
	CShaderNormalMap() {}
	~CShaderNormalMap(){}

	void Init( const char* _VertexShader, const char* _PixelShader );
	void Uninit();
	void Set();

	void SetMaterial(MATERIAL Material);
	void SetLight(LIGHT Light);

	void SetWorldMatrix(XMFLOAT4X4* _WorldMatrix) { m_Constant.WorldMatrix = Transpose(_WorldMatrix); }
	void SetViewMatrix(XMFLOAT4X4* _ViewMatrix) { m_Constant.ViewMatrix = Transpose(_ViewMatrix); }
	void SetProjectionMatrix(XMFLOAT4X4* ProjectionMatrix) { m_Constant.ProjectionMatrix = Transpose( ProjectionMatrix ); }
	void SetMtxWIT(XMFLOAT4X4* _MtxWIT) { m_Constant.MtxWIT = Transpose(_MtxWIT); }
	void SetCameraPosition(XMFLOAT4* _CameraPos) { m_Constant.CameraPosition = *_CameraPos; }
	void SetSpequlerPow(float _SpequlerPow) { m_Constant.SpequlerPow = _SpequlerPow; }

private:
	struct CONSTANT
	{
		XMFLOAT4X4 WorldMatrix;
		XMFLOAT4X4 ViewMatrix;
		XMFLOAT4X4 ProjectionMatrix;
		XMFLOAT4X4 MtxWIT;
		XMFLOAT4   CameraPosition;
		float      SpequlerPow;
		float      Dummy[3];
	};

	CONSTANT	m_Constant;

};

#endif // !SHADER_NORMAL_MAPPING_H_

