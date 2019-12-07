#include "main.h"
#include "shader_base.h"
#include "shader_multi_texture.h"
#include <io.h>

void CShaderMultiTexture::Init(const char* VertexShader, const char* PixelShader)
{
	// 頂点シェーダ生成
	{
		FILE* file;
		long int fsize;

		file = fopen(VertexShader, "rb");
		fsize = _filelength(_fileno(file));
		unsigned char* buffer = new unsigned char[fsize];
		fread(buffer, fsize, 1, file);
		fclose(file);

		CRenderer::GetDevice()->CreateVertexShader(buffer, fsize, NULL, &m_VertexShader);


		// 入力レイアウト生成
		{
			D3D11_INPUT_ELEMENT_DESC layout[] =
			{
				//{ 構造体, セマンテックス番号, 型指定, パイプライン番号, データのオフセット, ..., ...}
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,      D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 4 * 3,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 6,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, 4 * 10, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "BLENDNUM", 0, DXGI_FORMAT_R32_FLOAT,          0, 4 * 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
			UINT numElements = ARRAYSIZE(layout);

			CRenderer::GetDevice()->CreateInputLayout(layout,
				numElements,
				buffer,
				fsize,
				&m_VertexLayout);
		}

		delete[] buffer;
	}



	// ピクセルシェーダ生成
	{
		FILE* file;
		long int fsize;

		file = fopen(PixelShader, "rb");
		fsize = _filelength(_fileno(file));
		unsigned char* buffer = new unsigned char[fsize];
		fread(buffer, fsize, 1, file);
		fclose(file);

		CRenderer::GetDevice()->CreatePixelShader(buffer, fsize, NULL, &m_PixelShader);

		delete[] buffer;
	}



	// 定数バッファ生成
	{
		D3D11_BUFFER_DESC hBufferDesc;
		hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		hBufferDesc.CPUAccessFlags = 0;
		hBufferDesc.MiscFlags = 0;
		hBufferDesc.StructureByteStride = sizeof(float);

		hBufferDesc.ByteWidth = sizeof(CONSTANT);
		CRenderer::GetDevice()->CreateBuffer(&hBufferDesc, NULL, &m_ConstantBuffer);

		hBufferDesc.ByteWidth = sizeof(MATERIAL);
		CRenderer::GetDevice()->CreateBuffer(&hBufferDesc, NULL, &m_MaterialBuffer);

		hBufferDesc.ByteWidth = sizeof(LIGHT);
		CRenderer::GetDevice()->CreateBuffer(&hBufferDesc, NULL, &m_LightBuffer);

		hBufferDesc.ByteWidth = sizeof(XMFLOAT4);
		CRenderer::GetDevice()->CreateBuffer(&hBufferDesc, NULL, &m_CameraPositionBuffer);
	}
}

void CShaderMultiTexture::Uninit()
{
	if (m_LightBuffer)		m_LightBuffer->Release();
	if (m_MaterialBuffer)	m_MaterialBuffer->Release();
	if (m_ConstantBuffer)	m_ConstantBuffer->Release();

	if (m_PixelShader)		m_PixelShader->Release();
	if (m_VertexLayout)		m_VertexLayout->Release();
	if (m_VertexShader)		m_VertexShader->Release();
}

void CShaderMultiTexture::Set()
{
	// シェーダ設定
	CRenderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	CRenderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);


	// 入力レイアウト設定
	CRenderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);


	// 定数バッファ更新
	CRenderer::GetDeviceContext()->UpdateSubresource(m_ConstantBuffer, 0, NULL, &m_Constant, 0, 0);
	CRenderer::GetDeviceContext()->UpdateSubresource(m_CameraPositionBuffer, 0, NULL, &m_CameraPosition, 0, 0);

	// 定数バッファ設定
	CRenderer::GetDeviceContext()->VSSetConstantBuffers(0, 1, &m_ConstantBuffer);

	CRenderer::GetDeviceContext()->VSSetConstantBuffers(1, 1, &m_LightBuffer);

	CRenderer::GetDeviceContext()->VSSetConstantBuffers(2, 1, &m_MaterialBuffer);

	CRenderer::GetDeviceContext()->PSSetConstantBuffers(0, 1, &m_CameraPositionBuffer);
}

