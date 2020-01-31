#ifndef SHADER_2D_H_
#define SHADER_2D_H_

// 頂点構造体
struct VERTEX_2D
{
	Vector3  Position;
	XMFLOAT4 Diffuse;
	XMFLOAT2 TexCoord;
};

class CShader2D : public CShaderBase
{
public:
	CShader2D(){}
	~CShader2D(){}

	void Init(const char* VertexShader, const char* PixelShader);
	void Uninit();
	void Set();

private:

	// コンスタントバッファのデータ群
	struct CONSTANT
	{
		XMFLOAT4X4 ProjectionMatrix;
	};

	CONSTANT	m_Constant;

};

#endif // !SHADER_2D_H_

