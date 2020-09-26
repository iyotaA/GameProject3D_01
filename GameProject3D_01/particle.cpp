// =========================================================================
// GPUﾊﾟﾃｲｸﾙシステム
// by: JIWANGGA DIWANGKARA, 2019/12/01
// GPL [https://www.gnu.org/licenses/gpl-3.0.en.html]
// =========================================================================
//#include "particle.h"
//#include "GameObject.h"
//#include "manager.h"
//#include "shader.h"
//void CParticle::Init()
//{
//	m_Position = XMFLOAT3(14, 14, -64);
//	m_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
//	m_Scale = XMFLOAT3(4.0f, 4.0f, 4.0f);
//
//	//テクスチャ管理システム（自作）
//	m_Texture = new CTexture();
//	m_Texture = CTextureList::GetTextureList(0);
//
//	//バファー設定と初期化
//	D3D11_BUFFER_DESC bd;
//	bd = {};
//	bd.Usage = D3D11_USAGE_DEFAULT;
//	bd.ByteWidth = sizeof(m_Position) * 4;
//	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	bd.CPUAccessFlags = 0;
//	bd.StructureByteStride = 0;
//
//	//SUBRESOURCE設定と初期化
//	D3D11_SUBRESOURCE_DATA sd;
//	sd = {};
//	sd.pSysMem = pVertex;
//	sd.SysMemPitch = 0;
//	sd.SysMemSlicePitch = 0;
//	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
//
//	D3D11_INPUT_ELEMENT_DESC layout[] =
//	{
//		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//	};
//	CShader::LoadVertexShader("Particle_VShader.cso", &m_ParticleVShader, &m_ParticleVLayout, layout, ARRAYSIZE(layout));
//	CShader::LoadPixelShader("Particle_PShader.cso", &m_ParticlePShader);
//	CShader::LoadGeometryShader("Particle_GShader.cso", &m_ParticleGShader);
//
//
//}
//void CParticle::CreateInstance(int count)
//{
//	if (m_PositionBuffer == nullptr) {
//		m_instanceCount = count;
//		m_instanceData = new PARTICLE[m_instanceCount];
//		m_startFrame = new int[m_instanceCount];
//		for (int i = 0; i < m_instanceCount; i++) {
//			m_instanceData[i].x = 0;
//			m_instanceData[i].y = 0.0f;
//			m_instanceData[i].z = 0;
//		}
//		
//		D3D11_BUFFER_DESC hBufferDesc;
//		hBufferDesc.ByteWidth = sizeof(PARTICLE) * m_instanceCount;
//		hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
//		hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//		hBufferDesc.CPUAccessFlags = 0;
//		hBufferDesc.MiscFlags = 0;
//		hBufferDesc.StructureByteStride = sizeof(m_instanceData);
//		CRenderer::GetDevice()->CreateBuffer(&hBufferDesc, NULL, &m_PositionBuffer);
//		CRenderer::GetDeviceContext()->VSSetConstantBuffers(6, 1, &m_PositionBuffer);
//	}
//	//バファー作成に失敗したら、データを放棄する
//	if (m_PositionBuffer == nullptr) delete this;
//}
//void CParticle::Update()
//{
//		for (int i = 0; i < m_instanceCount; i++) {
//			//frameの値が-1以下の場合は、表示しないことにする
//			if (m_instanceData[i].frame < 0) {
//				m_instanceData[i].x = -std::numeric_limits<float>::max();
//				m_instanceData[i].y = -std::numeric_limits<float>::max();
//				m_instanceData[i].z = -std::numeric_limits<float>::max();
//			}
//			//frameの値が0の場合は、値を初期化する
//			if (m_instanceData[i].frame == 0) {
//				m_instanceData[i].x = 0;
//				m_instanceData[i].y = 0;
//				m_instanceData[i].z = 0;
//			}
//			//frameの値が1以上の場合は、位置値を格納したm_movementの値で足し続ける
//			if (m_instanceData[i].frame > 0) {
//				m_instanceData[i].x += m_movement[i].x;
//				m_instanceData[i].y += m_movement[i].y;
//				m_instanceData[i].z += m_movement[i].z;
//			}
//			m_instanceData[i].frame += 1;
//			//frameの値がm_maxFrameにたどり着いたら、初期化値に戻す
//			if (m_instanceData[i].frame > m_maxFrame) {
//				m_instanceData[i].frame = m_startFrame[i];
//			}
//	}
//	//GPUにデータを送る
//	CRenderer::GetDeviceContext()->UpdateSubresource(m_PositionBuffer, 0, NULL, m_instanceData, 0, 0);
//
//}
//void CParticle::Draw()
//{
//	//頂点バッファ初期化
//	UINT stride = sizeof(m_Position);
//	UINT offset = 0;
//	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
//	//ワルドマトリクス初期化
//	XMMATRIX world;
//	world = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
//	world *= XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
//	world *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
//	CRenderer::SetWorldMatrix(&world);																
//
//	//テクスチャ設定
//	CRenderer::SetTexture(m_Texture);
//
//	//ポイントリストにしないといけない
//	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
//
//	//シェーダーを適用する関数（自作）
//	CRenderer::SetCustomShader(m_ParticleVShader, m_ParticleVLayout, m_ParticlePShader);
//	CRenderer::GetDeviceContext()->GSSetShader(m_ParticleGShader, NULL, 0);
//
//	//インスタンス描画
//	CRenderer::GetDeviceContext()->DrawInstanced(1, m_instanceCount,0, 0);
//
//	//GSを使い終わったら、NULLにする
//	CRenderer::GetDeviceContext()->GSSetShader(nullptr, NULL, 0);
//
//	//元のシェーダーに戻す関数（自作）
//	CRenderer::SetDefaultShader();
//}
//
//void CParticle::Finalize()
//{
//	if (m_PositionBuffer) m_PositionBuffer->Release();
//	//	m_Texture->Unload();
//	delete m_Texture;
//}
