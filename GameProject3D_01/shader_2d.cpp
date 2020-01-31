#include "main.h"
#include "shader_base.h"
#include "shader_2d.h"
#include <io.h>

void CShader2D::Init(const char* VertexShader, const char* PixelShader)
{
	// ���_�V�F�[�_����
	{
		FILE* file;
		long int fsize;

		file = fopen(VertexShader, "rb");
		fsize = _filelength(_fileno(file));
		unsigned char* buffer = new unsigned char[fsize];
		fread(buffer, fsize, 1, file);
		fclose(file);

		CRenderer::GetDevice()->CreateVertexShader(buffer, fsize, NULL, &m_VertexShader);


		// ���̓��C�A�E�g����
		{
			D3D11_INPUT_ELEMENT_DESC layout[] =
			{
				//{ �\����, �Z�}���e�b�N�X�ԍ�, �^�w��, �p�C�v���C���ԍ�, �f�[�^�̃I�t�Z�b�g, ..., ...}
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,      D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 3,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, 4 * 7,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
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



	// �s�N�Z���V�F�[�_����
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



	// �萔�o�b�t�@����
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
	}

	// �v���W�F�N�V�����}�g���N�X�ݒ�
	XMFLOAT4X4 projection;
	XMStoreFloat4x4(&projection, XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f));
	m_Constant.ProjectionMatrix = Transpose(&projection);
}

void CShader2D::Uninit()
{
	if (m_LightBuffer)		m_LightBuffer->Release();
	if (m_MaterialBuffer)	m_MaterialBuffer->Release();
	if (m_ConstantBuffer)	m_ConstantBuffer->Release();

	if (m_PixelShader)		m_PixelShader->Release();
	if (m_VertexLayout)		m_VertexLayout->Release();
	if (m_VertexShader)		m_VertexShader->Release();
}

void CShader2D::Set()
{
	// �V�F�[�_�ݒ�
	CRenderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	CRenderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// ���̓��C�A�E�g�ݒ�
	CRenderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// �萔�o�b�t�@�X�V
	CRenderer::GetDeviceContext()->UpdateSubresource(m_ConstantBuffer, 0, NULL, &m_Constant, 0, 0);

	// �萔�o�b�t�@�ݒ�
	CRenderer::GetDeviceContext()->VSSetConstantBuffers(0, 1, &m_ConstantBuffer);

	CRenderer::GetDeviceContext()->VSSetConstantBuffers(1, 1, &m_LightBuffer);

	CRenderer::GetDeviceContext()->VSSetConstantBuffers(2, 1, &m_MaterialBuffer);
}
