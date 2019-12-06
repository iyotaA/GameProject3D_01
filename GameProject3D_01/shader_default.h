#ifndef SHADER_DEFAULT_H_
#define SHADER_DEFAULT_H_

struct CONSTANT_D
{
	XMFLOAT4X4 WorldMatrix;
	XMFLOAT4X4 ViewMatrix;
	XMFLOAT4X4 ProjectionMatrix;
};

class CShaderDefault : public ShaderBase
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

private:
	CONSTANT_D	m_Constant;

};

#endif // !SHADER_DEFAULT_H_

