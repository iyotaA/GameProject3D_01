#ifndef SHADER_MULTI_TEXTURE_H_
#define SHADER_MULTI_TEXTURE_H_

// 頂点構造体
struct VERTEX_3D_TEX2
{
	Vector3  Position;
	Vector3  Normal;
	XMFLOAT4 Diffuse;
	XMFLOAT2 TexCoord;
	float    BlendNum;
};

class CShaderMultiTexture : public CShaderBase
{
public:
	void Init(const char* VertexShader, const char* PixelShader);
	void Uninit();
	void Set();

	void SetWorldMatrix(XMFLOAT4X4* _WorldMatrix) { m_Constant.WorldMatrix = Transpose(_WorldMatrix); }
	void SetViewMatrix(XMFLOAT4X4* _ViewMatrix) { m_Constant.ViewMatrix = Transpose(_ViewMatrix); }
	void SetProjectionMatrix(XMFLOAT4X4* _ProjectionMatrix) { m_Constant.ProjectionMatrix = Transpose(_ProjectionMatrix); }
	void SetCameraPosition(XMFLOAT4* _CameraPosition) { m_CameraPosition = *_CameraPosition; }

private:
	// コンスタントバッファのデータ群
	struct CONSTANT
	{
		XMFLOAT4X4 WorldMatrix;
		XMFLOAT4X4 ViewMatrix;
		XMFLOAT4X4 ProjectionMatrix;
	};

	CONSTANT	  m_Constant;
	XMFLOAT4	  m_CameraPosition;
	ID3D11Buffer* m_CameraPositionBuffer;

};


#endif // !SHADER_MULTI_TEXTURE_H_

