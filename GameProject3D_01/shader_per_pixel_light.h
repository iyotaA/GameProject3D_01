#ifndef SHADER_PER_PIXEL_LIGHT_H_
#define SHADER_PER_PIXEL_LIGHT_H_


class CShaderPerPixelLight : public CShaderBase
{
public:
	CShaderPerPixelLight() {}
	~CShaderPerPixelLight() {}

	void Init(const char* VertexShader, const char* PixelShader);
	void Uninit();
	void Set();

	void SetWorldMatrix(XMFLOAT4X4* _WorldMatrix) { m_Constant.WorldMatrix = Transpose(_WorldMatrix); }
	void SetViewMatrix(XMFLOAT4X4* _ViewMatrix) { m_Constant.ViewMatrix = Transpose(_ViewMatrix); }
	void SetProjectionMatrix(XMFLOAT4X4* _ProjectionMatrix) { m_Constant.ProjectionMatrix = Transpose(_ProjectionMatrix); }
	void SetWITMatrix(XMFLOAT4X4* _WITMatrix) { m_Constant.WorldInverseTransposeMatrix = Transpose(_WITMatrix); }
	void SetCameraPosition(XMFLOAT4* _CameraPosition) { m_Constant.CameraPosition = *_CameraPosition; }

private:

	// コンスタントバッファのデータ群
	struct CONSTANT
	{
		XMFLOAT4X4	WorldMatrix;
		XMFLOAT4X4	ViewMatrix;
		XMFLOAT4X4	ProjectionMatrix;
		XMFLOAT4X4	WorldInverseTransposeMatrix;
		XMFLOAT4	CameraPosition;
	};

	CONSTANT	m_Constant;

};

#endif // !SHADER_PER_PIXEL_LIGHT_H_


