#ifndef SHADER_SUPER_H_
#define SHADER_SUPER_H_


class ShaderBase
{
public:
	ShaderBase(){}
	~ShaderBase(){}

	virtual void Init( const char* VertexShader, const char* PixelShader ) = 0;
	virtual void Uninit() = 0;
	virtual void Set() = 0;

	void SetMaterial(MATERIAL Material);
	void SetLight(LIGHT Light);


	XMFLOAT4X4 Transpose(XMFLOAT4X4* Matrix);

protected:
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader*  m_PixelShader;
	ID3D11InputLayout*  m_VertexLayout;
	ID3D11Buffer*       m_MaterialBuffer;
	ID3D11Buffer*       m_LightBuffer;
	ID3D11Buffer*       m_ConstantBuffer;


};

#endif // !SHADER_SUPER_H_

