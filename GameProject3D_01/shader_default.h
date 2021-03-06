#ifndef SHADER_DEFAULT_H_
#define SHADER_DEFAULT_H_


class CShaderDefault : public CShaderBase
{
public:
	CShaderDefault(){}
	~CShaderDefault(){}

	void Init(const char* VertexShader, const char* PixelShader);
	void Uninit();
	void Set();

	void SetWorldMatrix(XMFLOAT4X4* _WorldMatrix)          { m_Constant.WorldMatrix      = Transpose(_WorldMatrix); }
	void SetViewMatrix(XMFLOAT4X4* _ViewMatrix)            { m_Constant.ViewMatrix       = Transpose(_ViewMatrix); }
	void SetProjectionMatrix(XMFLOAT4X4* _ProjectionMatrix){ m_Constant.ProjectionMatrix = Transpose(_ProjectionMatrix); }
	void SetCameraPositoin(XMFLOAT3* _CameraPosition){ m_Constant.CameraPosition = XMFLOAT4(_CameraPosition->x, _CameraPosition->y, _CameraPosition->z, 1.0f); }

private:

	// コンスタントバッファのデータ群
	struct CONSTANT
	{
		XMFLOAT4X4 WorldMatrix;
		XMFLOAT4X4 ViewMatrix;
		XMFLOAT4X4 ProjectionMatrix;
		XMFLOAT4   CameraPosition;
	};

	CONSTANT	m_Constant;

};

#endif // !SHADER_DEFAULT_H_

