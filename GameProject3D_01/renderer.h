#ifndef RENDERER_H_
#define RENDERER_H_

// 頂点構造体
struct VERTEX_3D
{
	Vector3  Position;
	Vector3  Normal;
    XMFLOAT4 Diffuse;
    XMFLOAT2 TexCoord;
};


// 色構造体
struct COLOR
{
	COLOR(){}
	COLOR( float _r, float _g, float _b, float _a )
	{
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}

	float r;
	float g;
	float b;
	float a;
};


// マテリアル構造体
struct MATERIAL
{
	COLOR		Ambient;
	COLOR		Diffuse;
	COLOR		Specular;
	COLOR		Emission;
	float		Shininess;
	float		Dummy[3];//16bit境界用
};


// マテリアル構造体
struct DX11_MODEL_MATERIAL
{
	MATERIAL		Material;
	class CTexture*	Texture;
};


// 描画サブセット構造体
struct DX11_SUBSET
{
	unsigned short	StartIndex;
	unsigned short	IndexNum;
	DX11_MODEL_MATERIAL	Material;
};

struct LIGHT
{
	LIGHT() :
		Direction(XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)),
		Diffuse(COLOR(1.0f, 1.0f, 1.0f, 1.0f)),
		Ambient(COLOR(0.5, 0.5, 0.5, 1.0f))
	{}

	XMFLOAT4	Direction;
	COLOR		Diffuse;
	COLOR		Ambient;
};


class CTexture;


class CRenderer
{
public:
	static void Init();
	static void Uninit();
	static void Begin();
	static void End();

	static void SetDepthEnable(bool Enable);
	static void SetWorldViewProjection2D();
	static void SetTexture( CTexture* Texture );
	static void SetTexture(CTexture* Texture, unsigned int Slot);
	static void SetTexture(CTexture** Texture, unsigned int Slot, unsigned int NumTextures);
	static void DrawIndexed( unsigned int IndexCount, unsigned int StartIndexLocation, int BaseVertexLocation );
	static void SetRasterizerState(D3D11_FILL_MODE _fill_mode, D3D11_CULL_MODE _cull_mode);

	// Zバッファ
	static void BeginDepth();
	static void SetDepthTexture(unsigned int _slot);


	static ID3D11Device* GetDevice( void ){ return m_D3DDevice; }
	static ID3D11DeviceContext* GetDeviceContext( void ){ return m_ImmediateContext; }

private:

	static D3D_FEATURE_LEVEL			m_FeatureLevel;

	static ID3D11Device*				m_D3DDevice;
	static ID3D11DeviceContext*			m_ImmediateContext;
	static IDXGISwapChain*				m_SwapChain;
	static ID3D11RenderTargetView*		m_RenderTargetView;
	static ID3D11DepthStencilView*		m_DepthStencilView;
	static ID3D11DepthStencilState*		m_DepthStateEnable;
	static ID3D11DepthStencilState*		m_DepthStateDisable;
	static ID3D11DepthStencilView*		m_LightDepthStencilView;
	static ID3D11ShaderResourceView*	m_LightDepthShaderResourceView;

	/*
		static XMMATRIX				m_WorldMatrix;
		static XMMATRIX				m_ViewMatrix;
		static XMMATRIX				m_ProjectionMatrix;
	*/

};

#endif
