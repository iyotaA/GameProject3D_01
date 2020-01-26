// ���C�u����
#pragma comment(lib, "assimp.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

// �C���N���[�h
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/matrix4x4.h>

#include "main.h"
#include "renderer.h"
#include "input.h"
#include "texture.h"
#include "gameObject.h"
#include "shader_all.h"
#include "skinmesh_animation.h"
#include "modelAnimation.h"
#include "camera_manager.h"
#include "camera.h"


/* Assimp�̍s��̍������Ԃ͈Ⴄ����C��t���悤�I�I
	 A *= B;
	 �{���� : A = A * B;
	 Assimp�� : A = B * A;
*/
//************************************************
// �t�@�C�����[�h
//************************************************
void CSkinModel::Load(char* pFileName, float size, char* pTexture, char* output_fileName)
{
	m_pScene = aiImportFile(pFileName, aiProcessPreset_TargetRealtime_MaxQuality);

	if (m_pScene == NULL)
	{
		assert(false);
	}


	// �e�{�[�����擾
	CreateBone(m_pScene->mRootNode);

	m_Size = size;
	m_Mesh = new MESH[m_pScene->mNumMeshes];

	// ���_������
	m_pDeformVertex = new std::vector<DEFORM_VERTEX>[m_pScene->mNumMeshes];
	for (int m = 0; m < m_pScene->mNumMeshes; m++) {

		aiMesh* pMesh = m_pScene->mMeshes[m];

		// �e���b�V���̒��_������
		for (int v = 0; v < pMesh->mNumVertices; v++) {

			DEFORM_VERTEX vertex;
			vertex.Position = pMesh->mVertices[v];
			vertex.DeformPosition = pMesh->mVertices[v];
			vertex.Normal = pMesh->mNormals[v];
			vertex.DeformNormal = pMesh->mNormals[v];
			vertex.BoneNum = 0;

			for (auto b = 0; b < 4; b++) {
				//vertex.BoneIndex[b];
				vertex.BoneName[b] = " ";
				vertex.BoneWeight[b] = 0.0f;
			}

			// ���X�g�ɒǉ�
			m_pDeformVertex[m].push_back(vertex);
		}

		// �e���b�V���Ɋ֌W�����{�[�����i�[
		for (int b = 0; b < pMesh->mNumBones; b++) {

			//if (!pMesh->HasBones) continue;

			aiBone* pBone = pMesh->mBones[b];
			m_Bone[pBone->mName.C_Str()].OffsetMatrix = pBone->mOffsetMatrix;
			// FBX�t�@�C���̓{�[���������ɑ����Ă��钸�_�̃C���f�b�N�X�������Ă���B
			// ���ꂾ�ƃQ�[���ł͕s�s���Ȃ̂ŋt�ɂ��Ă���B
			// ���_�������Ă���{�[���̃C���f�b�N�X�����悤�ɂ���I�I

			for (int w = 0; w < pBone->mNumWeights; w++) {

				aiVertexWeight weight = pBone->mWeights[w];

				assert(m_pDeformVertex[m][weight.mVertexId].BoneNum <= 4);

				m_pDeformVertex[m][weight.mVertexId].BoneWeight[m_pDeformVertex[m][weight.mVertexId].BoneNum] = weight.mWeight;		// �{�[���̓����ɑ΂���d��
				m_pDeformVertex[m][weight.mVertexId].BoneName[m_pDeformVertex[m][weight.mVertexId].BoneNum] = pBone->mName.C_Str(); // ������������{�[���̖��O
				m_pDeformVertex[m][weight.mVertexId].BoneNum++;																		//
			}
		}
	}

	// �e�m�[�h�������o��
	if (output_fileName != NULL) {
		ofstream outputfile(output_fileName);
		WritteName(m_pScene->mRootNode, &outputfile);
		outputfile.close();
	}

	//// �e�N�X�`���擾
	aiString path;

	for (int mat = 0; mat < m_pScene->mNumMaterials; mat++) {

		if (m_pScene->mMaterials[mat]->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {

			// fbx�t�@�C���̓����Ƀe�N�X�`���������Ă��邩�ۂ�
			if (path.data[0] == '*') {
				// fbx�t�@�C�����Ƀe�N�X�`������
				int id = atoi(&path.data[1]);
				m_Texture[path.data] = new CTexture();
				m_Texture[path.data]->loadTextureFromMemory((const unsigned char*)m_pScene->mTextures[id]->pcData, m_pScene->mTextures[id]->mWidth);
			}
			else {
				m_Texture[path.data] = new CTexture();
				m_Texture[path.data]->LoadSTB(pTexture);
				// fbx�t�@�C���O�Ƀe�N�X�`������
				//size_t pos = modelPath.find_last_of("\\/");				// ���f���̃p�X�̌�납��ŏ��ɏo�Ă���"\\/"�̏ꏊ���擾
				//std::string texPath = (char*)& path.data;					// Texture�̃p�X�擾
				//size_t texpos = texPath.find_last_of("\\/");				// Texture�̃p�X�̌�납��ŏ��ɏo�Ă���"\\/"�̏ꏊ���擾
				//std::string texturePath = modelPath.substr(0, pos + 1);		// ���f���Ɠ����ꏊ�̃p�X���擾
				//texturePath += texPath.substr(texpos + 1, sizeof(texPath));	// ���f���Ɠ����ꏊ�ɂ���e�N�X�`���̃p�X�ɏ�������
				//m_Texture[tex] = LoadTextureSTB(texturePath.c_str());		// �w�肵���p�X�̃e�N�X�`����ǂݍ���
			}
		}
		else {
			m_Texture["DefauldTexture"] = new CTexture();
			m_Texture["DefauldTexture"]->LoadSTB(pTexture);
		}
	}

	// �A�j���[�V�����f�[�^�����邩�H
	if (m_pScene->HasAnimations()) {
		LoadMesh(m_pScene->mRootNode);
	}
	else {
		LoadStaticMesh();
	}

	//DrawMesh(m_pScene->mRootNode, &aiMatrix4x4());

	m_Shader = ShaderManager::GetShader<CShaderDefault>();
}

//************************************************
// �i�A�j���[�V�����Ȃ��j���b�V�����ƂɃo�b�t�@�쐬
//************************************************
void CSkinModel::LoadStaticMesh()
{
	for (int m = 0; m < m_pScene->mNumMeshes; m++) {

		aiMesh* pMesh = m_pScene->mMeshes[m];

		// �}�e���A���擾
		const aiMaterial* mat = m_pScene->mMaterials[pMesh->mMaterialIndex];
		aiColor4D diffuse;
		aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &diffuse);	// �f�B�t�[�Y�J���[�擾

		VERTEX_3D* vertices = new VERTEX_3D[pMesh->mNumVertices];

		// Index���i�[
		UINT* Indices;
		Indices = new UINT[pMesh->mNumFaces * 3];
		m_Mesh[m].IndexNum = pMesh->mNumFaces * 3;

		// ���b�V���̖ʃ|���S���̌����J��Ԃ�
		for (int face = 0; face < pMesh->mNumFaces; face++) {
			const aiFace* pFace = &pMesh->mFaces[face];	// �ʃ|���S������̃f�[�^�擾

			assert(pFace->mNumIndices == 3);

			Indices[face * 3 + 0] = pFace->mIndices[0];
			Indices[face * 3 + 1] = pFace->mIndices[1];
			Indices[face * 3 + 2] = pFace->mIndices[2];

			std::vector<DEFORM_VERTEX>* pVertices = &m_pDeformVertex[m];

			// 1�̖ʃ|���S���̒��_�����擾
			for (int vertex = 0; vertex < pFace->mNumIndices; vertex++) {

				// �e���_���ɃA�N�Z�X
				int id = pFace->mIndices[vertex];

				// ���_���i�[
				vertices[id].Diffuse = XMFLOAT4(diffuse.r, diffuse.g, diffuse.b, 1.0f);
				vertices[id].Normal = Vector3(pMesh->mNormals[id].x, pMesh->mNormals[id].y, pMesh->mNormals[id].z);
				vertices[id].Position = Vector3(pMesh->mVertices[id].x, pMesh->mVertices[id].y, pMesh->mVertices[id].z);
				vertices[id].TexCoord = pMesh->HasTextureCoords(0) ? XMFLOAT2(pMesh->mTextureCoords[0][id].x, 1.0f - pMesh->mTextureCoords[0][id].y) : XMFLOAT2(0.0f, 0.0f);
			}
		}

		// VertexBuffer �쐬
		{
			D3D11_BUFFER_DESC vbDesc;
			vbDesc.ByteWidth = sizeof(VERTEX_3D) * pMesh->mNumVertices;
			vbDesc.Usage = D3D11_USAGE_DEFAULT;
			vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vbDesc.CPUAccessFlags = 0;;
			vbDesc.MiscFlags = 0;
			vbDesc.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA vrData;
			vrData.pSysMem = vertices;
			vrData.SysMemPitch = 0;
			vrData.SysMemSlicePitch = 0;

			CRenderer::GetDevice()->CreateBuffer(&vbDesc, &vrData, &m_Mesh[m].VertexBuffer);
		}

		// IndexBuffer �쐬
		{
			D3D11_BUFFER_DESC bd{};
			bd.ByteWidth = sizeof(UINT) * m_Mesh[m].IndexNum;
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;
			bd.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA sd{};
			sd.pSysMem = Indices;
			sd.SysMemPitch = 0;
			sd.SysMemSlicePitch = 0;

			CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_Mesh[m].IndexBuffer);
		}
		delete[] vertices;
		delete[] Indices;
	}
}


//************************************************
// �i�A�j���[�V��������j���b�V�����ƂɃo�b�t�@�쐬
//************************************************
void CSkinModel::LoadMesh(const aiNode* pNode)
{
	for (int mesh = 0; mesh < pNode->mNumMeshes; mesh++) {

		// ���b�V��1���擾
		const aiMesh* pMesh = m_pScene->mMeshes[pNode->mMeshes[mesh]];
		UINT mesh_index = pNode->mMeshes[mesh];
		m_Mesh[mesh_index].IndexNum = pMesh->mNumFaces * 3;

		// �}�e���A���擾
		const aiMaterial* mat = m_pScene->mMaterials[pMesh->mMaterialIndex];
		aiColor4D diffuse;
		aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &diffuse);	// �f�B�t�[�Y�J���[�擾

		VERTEX_3D* vertices = new VERTEX_3D[pMesh->mNumVertices];

		// Index���i�[
		UINT* Indices;
		Indices = new UINT[pMesh->mNumFaces * 3];

		// ���b�V���̖ʃ|���S���̌����J��Ԃ�
		for (int face = 0; face < pMesh->mNumFaces; face++) {
			const aiFace* pFace = &pMesh->mFaces[face];	// �ʃ|���S������̃f�[�^�擾

			assert(pFace->mNumIndices == 3);

			Indices[face * 3 + 0] = pFace->mIndices[0];
			Indices[face * 3 + 1] = pFace->mIndices[1];
			Indices[face * 3 + 2] = pFace->mIndices[2];

			std::vector<DEFORM_VERTEX>* pVertices = &m_pDeformVertex[mesh_index];

			// 1�̖ʃ|���S���̒��_�����擾
			for (int vertex = 0; vertex < pFace->mNumIndices; vertex++) {

				// �e���_���ɃA�N�Z�X
				int id = pFace->mIndices[vertex];
				DEFORM_VERTEX* pVertex = &(*pVertices)[id];

				assert(pVertex);

				// ���_���i�[
				vertices[id].Diffuse = XMFLOAT4(diffuse.r, diffuse.g, diffuse.b, 1.0f);
				vertices[id].Normal = Vector3(pVertex->Normal.x, pVertex->Normal.y, pVertex->Normal.z);
				vertices[id].Position = Vector3(pVertex->Position.x, pVertex->Position.y, pVertex->Position.z);
				vertices[id].TexCoord = pMesh->HasTextureCoords(0) ? XMFLOAT2(pMesh->mTextureCoords[0][id].x, 1.0f - pMesh->mTextureCoords[0][id].y) : XMFLOAT2(0.0f, 0.0f);
			}
		}

		// VertexBuffer �쐬
		{
			D3D11_BUFFER_DESC vbDesc;
			vbDesc.ByteWidth = sizeof(VERTEX_3D) * pMesh->mNumVertices;
			vbDesc.Usage = D3D11_USAGE_DYNAMIC;
			vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;;
			vbDesc.MiscFlags = 0;
			vbDesc.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA vrData;
			vrData.pSysMem = vertices;
			vrData.SysMemPitch = 0;
			vrData.SysMemSlicePitch = 0;

			CRenderer::GetDevice()->CreateBuffer(&vbDesc, &vrData, &m_Mesh[mesh_index].VertexBuffer);
		}

		// IndexBuffer �쐬
		{
			D3D11_BUFFER_DESC bd{};
			bd.ByteWidth = sizeof(UINT) * m_Mesh[mesh_index].IndexNum;
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;
			bd.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA sd{};
			sd.pSysMem = Indices;
			sd.SysMemPitch = 0;
			sd.SysMemSlicePitch = 0;

			CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_Mesh[mesh_index].IndexBuffer);
		}

		delete[] vertices;
		delete[] Indices;
	}


	for (int i = 0; i < pNode->mNumChildren; i++) {
		LoadMesh(pNode->mChildren[i]);
	}

}


//************************************************
// �t�@�C���A�����[�h
//************************************************
void CSkinModel::Unload()
{
	aiReleaseImport(m_pScene);
}


//************************************************
// �`��
//************************************************
void CSkinModel::Draw(XMMATRIX* world)
{
	XMMATRIX _world = *world;

	// XMMATRIX�s��̍���
	_world = XMMatrixScaling(m_Size, m_Size, m_Size) * _world;

	XMFLOAT4X4 world_4x4;
	XMStoreFloat4x4(&world_4x4, _world);

	CCamera* camera = CCameraManager::GetCamera();

	m_Shader->SetWorldMatrix(&world_4x4);
	m_Shader->SetViewMatrix(&camera->GetViewMatrix());
	m_Shader->SetProjectionMatrix(&camera->GetProjectionMatrix());
	m_Shader->SetLight(LIGHT());


	//CRenderer::SetWorldMatrix(&_world);

	// ���f������ŕ`�悷��
	if (m_DrawAtLine) {
		CRenderer::SetRasterizerState(D3D11_FILL_WIREFRAME, D3D11_CULL_NONE);
	}
	// ���f����ʂŕ`�悷��
	else {
		CRenderer::SetRasterizerState(D3D11_FILL_SOLID, D3D11_CULL_NONE);
	}

	// �A�j���[�V�����f�[�^�����ĂȂ�
	if (!m_pScene->HasAnimations()) {
		// �`��
		DrawStaticMesh();
	}
	else {
		// �`��
		DrawMesh(m_pScene->mRootNode);
	}

	CRenderer::SetRasterizerState(D3D11_FILL_SOLID, D3D11_CULL_NONE);
}

//************************************************
// �A�j���[�V�����������Ȃ����f���̕`��
//************************************************
void CSkinModel::DrawStaticMesh()
{
	for (int mesh = 0; mesh < m_pScene->mNumMeshes; mesh++) {

		// ���b�V��1���擾
		aiMesh* pMesh = m_pScene->mMeshes[mesh];

		UINT stride;
		UINT offset;

		stride = sizeof(VERTEX_3D);
		offset = 0;

		CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_Mesh[mesh].VertexBuffer, &stride, &offset);

		CRenderer::GetDeviceContext()->IASetIndexBuffer(m_Mesh[mesh].IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		const aiMaterial* mat = m_pScene->mMaterials[pMesh->mMaterialIndex];
		aiString path;
		if (mat->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {
			CRenderer::SetTexture(m_Texture[path.data], 0);
		}
		else {
			CRenderer::SetTexture(m_Texture["DefauldTexture"], 0);
		}

		aiColor4D diffuse, ambient;
		aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &diffuse);	// �f�B�t�[�Y�J���[�擾
		aiGetMaterialColor(mat, AI_MATKEY_COLOR_AMBIENT, &ambient);	// �f�B�t�[�Y�J���[�擾


		MATERIAL material;
		material.Diffuse = COLOR(diffuse.r, diffuse.g, diffuse.b, diffuse.a);

		material.Ambient = COLOR(ambient.r * 2, ambient.g * 2, ambient.b * 2, ambient.a);
		m_Shader->SetMaterial(material);
		m_Shader->Set();


		CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		CRenderer::DrawIndexed(m_Mesh[mesh].IndexNum, 0, 0);
	}
}

//************************************************
// ���b�V�����Ƃ̕`��
//************************************************
void CSkinModel::DrawMesh(const aiNode* pNode)
{
	for (int mesh = 0; mesh < pNode->mNumMeshes; mesh++) {

		// ���b�V��1���擾
		const aiMesh* pMesh = m_pScene->mMeshes[pNode->mMeshes[mesh]];
		UINT mesh_index = pNode->mMeshes[mesh];
		m_Mesh[mesh_index].IndexNum = pMesh->mNumFaces * 3;

		// ���b�V�����Ƃ̒��_�Q���擾
		VERTEX_3D* vertices = nullptr;
		vertices = new VERTEX_3D[pMesh->mNumVertices];
		assert(vertices);

		// ���b�V���̖ʃ|���S���̌����J��Ԃ�
		for (int face = 0; face < pMesh->mNumFaces; face++) {

			const aiFace* pFace = &pMesh->mFaces[face];	// �ʃ|���S������̃f�[�^�擾

			assert(pFace->mNumIndices == 3);

			std::vector<DEFORM_VERTEX>* pVertices = &m_pDeformVertex[mesh_index];

			// 1�̖ʃ|���S���̒��_�����擾
			for (int vertex_id = 0; vertex_id < pFace->mNumIndices; vertex_id++) {

				// �e���_���ɃA�N�Z�X
				int id = pFace->mIndices[vertex_id];
				DEFORM_VERTEX* pVertex = &(*pVertices)[id];

				assert(pVertex);

				// ���_���i�[
				vertices[id].Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
				vertices[id].Normal   = Vector3(-pVertex->DeformNormal.x, pVertex->DeformNormal.y, pVertex->DeformNormal.z);
				vertices[id].Position = Vector3(-pVertex->DeformPosition.x, pVertex->DeformPosition.y, pVertex->DeformPosition.z);
				vertices[id].TexCoord = pMesh->HasTextureCoords(0) ? XMFLOAT2(pMesh->mTextureCoords[0][id].x, 1.0f - pMesh->mTextureCoords[0][id].y) : XMFLOAT2(0.0f, 0.0f);
			}
		}


		// VertexBuffer��������
		{
			D3D11_MAPPED_SUBRESOURCE msr;
			CRenderer::GetDeviceContext()->Map(m_Mesh[mesh_index].VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
			memcpy(msr.pData, vertices, sizeof(VERTEX_3D) * pMesh->mNumVertices);
			CRenderer::GetDeviceContext()->Unmap(m_Mesh[mesh_index].VertexBuffer, 0);
		}

		delete[] vertices;


		{
			// �}�e���A���擾
			const aiMaterial* mat = m_pScene->mMaterials[pMesh->mMaterialIndex];
			assert(mat);
			aiColor4D diffuse, ambient;
			aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &diffuse);	// �f�B�t�[�Y�J���[�擾
			aiGetMaterialColor(mat, AI_MATKEY_COLOR_AMBIENT, &ambient);	// �f�B�t�[�Y�J���[�擾


			MATERIAL material;
			material.Diffuse = COLOR(diffuse.r, diffuse.g, diffuse.b, diffuse.a);

			material.Ambient = COLOR(ambient.r * 2, ambient.g * 2, ambient.b * 2, ambient.a);

			m_Shader->SetMaterial(material);
			m_Shader->Set();

			aiString path;
			mat->GetTexture(aiTextureType_DIFFUSE, 0, &path);
			CRenderer::SetTexture(m_Texture[m_pScene->mRootNode->mName.C_Str()]);
		}

		{
			UINT Stride = sizeof(VERTEX_3D);
			UINT offdet = 0;
			CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_Mesh[mesh_index].VertexBuffer, &Stride, &offdet);	// �o�[�e�N�X�o�b�t�@�Z�b�g
			CRenderer::GetDeviceContext()->IASetIndexBuffer(m_Mesh[mesh_index].IndexBuffer, DXGI_FORMAT_R32_UINT, 0);		// �C���f�b�N�X�o�b�t�@�Z�b�g
		}

		CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		CRenderer::DrawIndexed( m_Mesh[mesh_index].IndexNum, 0, 0 );
		//CRenderer::GetDeviceContext()->DrawIndexedInstanced(m_Mesh[mesh_index].IndexNum, 100, 0, 0, 0);
	}


	// �q�`��
	for (int i = 0; i < pNode->mNumChildren; i++) {
		DrawMesh(pNode->mChildren[i]);
	}
}


//************************************************
// �{�[���̏�����
//************************************************
void CSkinModel::CreateBone(aiNode* pNode)
{
	BONE bone;
	m_Bone[pNode->mName.C_Str()] = bone; // ��������������Ă��Ȃ��S�~�f�[�^�����邱�Ƃ�"��"������Ă���
	m_Texture[pNode->mName.C_Str()] = new CTexture();
	m_Texture[pNode->mName.C_Str()]->LoadSTB("asset/image/white.png");

	for (int i = 0; i < pNode->mNumChildren; i++) {
		CreateBone(pNode->mChildren[i]);
	}
}


//************************************************
// �A�j���[�V�����f�[�^�X�V
//************************************************
void CSkinModel::update(int addAnimationFrame)
{
	// �A�j���[�V�����f�[�^�������Ă��邩
	if (!m_pScene->HasAnimations()) return;

	// �A�j���[�V�����u�����h���邩�H���Ȃ����H
	m_IsAnimationBlending ? AnimationBlend(addAnimationFrame) : Animation(addAnimationFrame);

	// �{�[���̊e�s����v�Z
	CalculateBoneMatrix();

	// �ċA�I�Ƀ{�[���f�[�^�X�V
	UpdateBoneMatrix(m_pScene->mRootNode, aiMatrix4x4());
}


void CSkinModel::CalculateBoneMatrix()
{
	// �e���_�̍��W�v�Z�i�{���Ȃ�V�F�[�_�[�����ׂ��j
/*
  ���钸�_��ϊ�����s��� =
  A * aw + B * bw + C * cw + D * dw		(aw + bw + cw + dw == 1.0f)

  ( �啶�� : �֘A����{�[��, ������ : ���̃E�F�C�g )
*/
	for (UINT m = 0; m < m_pScene->mNumMeshes; m++)
	{
		for (auto& vertex : m_pDeformVertex[m])
		{
			aiMatrix4x4 matrix[4];
			aiMatrix4x4 outMatrix;
			matrix[0] = m_Bone[vertex.BoneName[0]].Matrix;
			matrix[1] = m_Bone[vertex.BoneName[1]].Matrix;
			matrix[2] = m_Bone[vertex.BoneName[2]].Matrix;
			matrix[3] = m_Bone[vertex.BoneName[3]].Matrix;

			//�E�F�C�g���l�����ă}�g���N�X�Z�o
			{
				outMatrix.a1 = matrix[0].a1 * vertex.BoneWeight[0]
					+ matrix[1].a1 * vertex.BoneWeight[1]
					+ matrix[2].a1 * vertex.BoneWeight[2]
					+ matrix[3].a1 * vertex.BoneWeight[3];

				outMatrix.a2 = matrix[0].a2 * vertex.BoneWeight[0]
					+ matrix[1].a2 * vertex.BoneWeight[1]
					+ matrix[2].a2 * vertex.BoneWeight[2]
					+ matrix[3].a2 * vertex.BoneWeight[3];

				outMatrix.a3 = matrix[0].a3 * vertex.BoneWeight[0]
					+ matrix[1].a3 * vertex.BoneWeight[1]
					+ matrix[2].a3 * vertex.BoneWeight[2]
					+ matrix[3].a3 * vertex.BoneWeight[3];

				outMatrix.a4 = matrix[0].a4 * vertex.BoneWeight[0]
					+ matrix[1].a4 * vertex.BoneWeight[1]
					+ matrix[2].a4 * vertex.BoneWeight[2]
					+ matrix[3].a4 * vertex.BoneWeight[3];



				outMatrix.b1 = matrix[0].b1 * vertex.BoneWeight[0]
					+ matrix[1].b1 * vertex.BoneWeight[1]
					+ matrix[2].b1 * vertex.BoneWeight[2]
					+ matrix[3].b1 * vertex.BoneWeight[3];

				outMatrix.b2 = matrix[0].b2 * vertex.BoneWeight[0]
					+ matrix[1].b2 * vertex.BoneWeight[1]
					+ matrix[2].b2 * vertex.BoneWeight[2]
					+ matrix[3].b2 * vertex.BoneWeight[3];

				outMatrix.b3 = matrix[0].b3 * vertex.BoneWeight[0]
					+ matrix[1].b3 * vertex.BoneWeight[1]
					+ matrix[2].b3 * vertex.BoneWeight[2]
					+ matrix[3].b3 * vertex.BoneWeight[3];

				outMatrix.b4 = matrix[0].b4 * vertex.BoneWeight[0]
					+ matrix[1].b4 * vertex.BoneWeight[1]
					+ matrix[2].b4 * vertex.BoneWeight[2]
					+ matrix[3].b4 * vertex.BoneWeight[3];



				outMatrix.c1 = matrix[0].c1 * vertex.BoneWeight[0]
					+ matrix[1].c1 * vertex.BoneWeight[1]
					+ matrix[2].c1 * vertex.BoneWeight[2]
					+ matrix[3].c1 * vertex.BoneWeight[3];

				outMatrix.c2 = matrix[0].c2 * vertex.BoneWeight[0]
					+ matrix[1].c2 * vertex.BoneWeight[1]
					+ matrix[2].c2 * vertex.BoneWeight[2]
					+ matrix[3].c2 * vertex.BoneWeight[3];

				outMatrix.c3 = matrix[0].c3 * vertex.BoneWeight[0]
					+ matrix[1].c3 * vertex.BoneWeight[1]
					+ matrix[2].c3 * vertex.BoneWeight[2]
					+ matrix[3].c3 * vertex.BoneWeight[3];

				outMatrix.c4 = matrix[0].c4 * vertex.BoneWeight[0]
					+ matrix[1].c4 * vertex.BoneWeight[1]
					+ matrix[2].c4 * vertex.BoneWeight[2]
					+ matrix[3].c4 * vertex.BoneWeight[3];



				outMatrix.d1 = matrix[0].d1 * vertex.BoneWeight[0]
					+ matrix[1].d1 * vertex.BoneWeight[1]
					+ matrix[2].d1 * vertex.BoneWeight[2]
					+ matrix[3].d1 * vertex.BoneWeight[3];

				outMatrix.d2 = matrix[0].d2 * vertex.BoneWeight[0]
					+ matrix[1].d2 * vertex.BoneWeight[1]
					+ matrix[2].d2 * vertex.BoneWeight[2]
					+ matrix[3].d2 * vertex.BoneWeight[3];

				outMatrix.d3 = matrix[0].d3 * vertex.BoneWeight[0]
					+ matrix[1].d3 * vertex.BoneWeight[1]
					+ matrix[2].d3 * vertex.BoneWeight[2]
					+ matrix[3].d3 * vertex.BoneWeight[3];

				outMatrix.d4 = matrix[0].d4 * vertex.BoneWeight[0]
					+ matrix[1].d4 * vertex.BoneWeight[1]
					+ matrix[2].d4 * vertex.BoneWeight[2]
					+ matrix[3].d4 * vertex.BoneWeight[3];

			}

			vertex.DeformPosition = vertex.Position;
			vertex.DeformPosition *= outMatrix;


			//�@���ϊ��p�Ɉړ��������폜
			outMatrix.a4 = 0.0f;
			outMatrix.b4 = 0.0f;
			outMatrix.c4 = 0.0f;

			vertex.DeformNormal = vertex.Normal;
			vertex.DeformNormal *= outMatrix;
		}
	}
}


void CSkinModel::AnimationBlend(int addAnimationFrame)
{
	// �A�j���[�V�����f�[�^�擾									���ǂ̃A�j���\�V�����H
	aiAnimation* pAnimationCurrent = m_pScene->mAnimations[m_CurrentAnimId];
	aiAnimation* pAnimationTarget = m_pScene->mAnimations[m_TargetAnimId];

	// �A�j���[�V�����L�[�X�V
	m_CurrentAnimationFrame += addAnimationFrame * m_AnimationSpeed;
	m_TargetAnimationFrame += addAnimationFrame * m_AnimationSpeed;

	for (auto c = 0; c < pAnimationCurrent->mNumChannels; c++) {

		// ���݃t���[���̃A�j���[�V�����s��̉�]�E���s�ړ��������擾
		int CurrentFrame = m_IsStopMotion ? (int)pAnimationCurrent->mDuration * (m_MotionFrame / 100.0f) : static_cast<int>(m_CurrentAnimationFrame);
		int CurrentFrameTarget = m_IsStopMotion ? (int)pAnimationCurrent->mDuration * (m_MotionFrame / 100.0f) : static_cast<int>(m_TargetAnimationFrame);

		// �m�[�h�A�j���[�V�����擾
		aiNodeAnim* pNodeAnimCurrent = pAnimationCurrent->mChannels[c];
		aiNodeAnim* pNodeAnimTarget = pAnimationTarget->mChannels[c];
		BONE* pBone = &m_Bone[pNodeAnimCurrent->mNodeName.C_Str()];

		aiQuaternion rotCurrent, rotTarget, rotBlend;
		aiVector3D posCurrent, posTarget, posBlend;

		rotCurrent = pNodeAnimCurrent->mRotationKeys[CurrentFrame % pNodeAnimCurrent->mNumRotationKeys].mValue;
		posCurrent = pNodeAnimCurrent->mPositionKeys[CurrentFrame % pNodeAnimCurrent->mNumPositionKeys].mValue;
		rotTarget = pNodeAnimTarget->mRotationKeys[CurrentFrameTarget % pNodeAnimTarget->mNumRotationKeys].mValue;
		posTarget = pNodeAnimTarget->mPositionKeys[CurrentFrameTarget % pNodeAnimTarget->mNumPositionKeys].mValue;


		// ��]���u�����h
		aiQuaternion::Interpolate(rotBlend, rotCurrent, rotTarget, m_PerBlend);

		// �|�W�V�������u�����h
		aiVector3D_Lerp(posBlend, posCurrent, posTarget, m_PerBlend);

		// �s��ɂ��ăN�H�[�^�j�I���Ƃ��Ċi�[
		pBone->AnimationMatrix = aiMatrix4x4(
			aiVector3D(1.0f, 1.0f, 1.0f), // �g��k��
			rotBlend,					  // ��]
			posBlend                      // �ړ�
		);
	}


	// �u�����h�l���X�V
	m_PerBlend += 0.1f;
	if (m_PerBlend >= m_PerBlendEnd) {
		m_IsAnimationBlending = false;
		m_CurrentAnimationFrame = m_TargetAnimationFrame;

		m_CurrentAnimId = m_TargetAnimId;
		m_PerBlend = 0.0f;
	}

	//m_PerBlend = (m_PerBlend >= 1.0f)? 1.0f: m_PerBlend;

	//if (m_TargetAnimationFrame >= (int)pAnimationTarget->mDuration) {
	//	m_IsAnimationBlending = false;
	//	m_CurrentAnimationFrame = m_TargetAnimationFrame = 0;

	//	m_CurrentAnimId = m_TargetAnimId;
	//	m_PerBlend = 0.0f;
	//}

}


void CSkinModel::Animation(int addAnimationFrame)
{
	// �A�j���[�V�����L�[�X�V
	m_CurrentAnimationFrame += addAnimationFrame * m_AnimationSpeed;

	// �A�j���[�V�����f�[�^�擾									���ǂ̃A�j���\�V�����H
	aiAnimation* pAnimationCurrent = m_pScene->mAnimations[m_CurrentAnimId];

	for (auto c = 0; c < pAnimationCurrent->mNumChannels; c++) {

		// ���݃t���[���̃A�j���[�V�����s��̉�]�E���s�ړ��������擾
		int CurrentFrame = m_IsStopMotion ? (int)pAnimationCurrent->mDuration * (m_MotionFrame / 100.0f) : static_cast<int>(m_CurrentAnimationFrame);

		// �m�[�h�A�j���[�V�����擾
		aiNodeAnim* pNodeAnimCurrent = pAnimationCurrent->mChannels[c];
		BONE* pBone = &m_Bone[pNodeAnimCurrent->mNodeName.C_Str()];

		aiQuaternion rotCurrent;
		aiVector3D posCurrent;

		rotCurrent = pNodeAnimCurrent->mRotationKeys[CurrentFrame % pNodeAnimCurrent->mNumRotationKeys].mValue;
		posCurrent = pNodeAnimCurrent->mPositionKeys[CurrentFrame % pNodeAnimCurrent->mNumPositionKeys].mValue;

		// �s��ɂ��ăN�H�[�^�j�I���Ƃ��Ċi�[
		pBone->AnimationMatrix = aiMatrix4x4(
			aiVector3D(1.0f, 1.0f, 1.0f), // �g��k��
			rotCurrent,					  // ��]
			posCurrent                      // �ړ�
		);
	}
}

//************************************************
// �{�[���s��X�V
//************************************************
void CSkinModel::UpdateBoneMatrix(aiNode* pNode, aiMatrix4x4 matrix)
{
	BONE* pBone = &m_Bone[pNode->mName.C_Str()];

	aiMatrix4x4 worldMatrix;
	worldMatrix = matrix;
	worldMatrix *= pBone->AnimationMatrix;
	pBone->Matrix = worldMatrix;
	pBone->Matrix *= pBone->OffsetMatrix;

	for (int n = 0; n < pNode->mNumChildren; n++) {

		UpdateBoneMatrix(pNode->mChildren[n], worldMatrix);
	}
}

//************************************************
// ���O����m�[�h���Q�b�g
//************************************************
aiNode* CSkinModel::GetBoneNode(aiNode* pNode, const char* _name)
{
	string thisNodeName = pNode->mName.C_Str();
	string Name = _name;

	if (thisNodeName.compare(Name) == 0) {
		return pNode;
	}

	// �q�T��
	for (int i = 0; i < pNode->mNumChildren; i++) {

		aiNode* node = GetBoneNode(pNode->mChildren[i], _name);

		if (node != NULL) {

			return node;
		}
	}

	return NULL;
}

//************************************************
// ���O���烏�[���h���W���Q�b�g
//************************************************
Vector3 CSkinModel::GetWorldPosition(XMMATRIX* _world, const char* _bone_name)
{
	XMMATRIX mtxWorld;
	mtxWorld = *_world;

	//���f����Ԃ̃{�[���ʒu�i�s��j
	XMMATRIX targetMatrix = XMMatrixIdentity();
	GetBonePosition(m_pScene->mRootNode, &XMMatrixIdentity(), _bone_name, targetMatrix);

	// ���[���h��Ԃ̃{�[���ʒu�i�s��j
	targetMatrix = targetMatrix * XMMatrixScaling(m_Size, m_Size, m_Size) * *_world;

	// ���[���h��Ԃ̃{�[���̈ʒu
	XMVECTOR bonePosition = {0.0f, 0.0f, 0.0f, 0.0f};
	bonePosition = XMVector3Transform(bonePosition, targetMatrix);

	return bonePosition;
}


//************************************************
// �w�肵���{�[���̃��[���h�s����擾
//************************************************
void CSkinModel::GetBonePosition(aiNode* pNode, XMMATRIX* _world, const char* _targetName, XMMATRIX& _target)
{
	// �e��Ԃ̎p���s��
	XMMATRIX mtxWorld = *_world;

	// ���[�J����Ԃł̎p���s�� * �e��Ԃ̐e�p���s�� = �e��Ԃ̃{�[���p��
	mtxWorld = XMLoadFloat4x4( &LoadAiMatrix4x4(&m_Bone[pNode->mName.C_Str()].AnimationMatrix)) * mtxWorld;

	// �Ώۂ̃m�[�h��������^�[�Q�b�g�s��ɃZ�b�g
	string name = _targetName;
	if (name.compare(pNode->mName.C_Str()) == 0) {
		_target = mtxWorld;
		return;
	}

	// �q�m�[�h�ɃA�N�Z�X
	for (int i = 0; i < pNode->mNumChildren; i++) {
		GetBonePosition(pNode->mChildren[i], &mtxWorld, _targetName, _target);
	}
}

bool CSkinModel::CurrentAnimationFinish()
{
	aiAnimation* pAnimationCurrent = m_pScene->mAnimations[m_CurrentAnimId];

	return (m_CurrentAnimationFrame >= m_pScene->mAnimations[m_CurrentAnimId]->mDuration) ? true : false;
}

void CSkinModel::SetAnimation(bool _next)
{
	m_PerBlend = 0.0f;
	m_TargetAnimId = m_CurrentAnimId;
	m_TargetAnimId += _next ? 1 : -1;

	if (m_TargetAnimId == m_pScene->mNumAnimations) {
		m_TargetAnimId = 0;
	}
	else if (m_TargetAnimId < 0) {
		m_TargetAnimId = m_pScene->mNumAnimations - 1;
	}
}

void CSkinModel::SetAnimation(const unsigned int _id, const float _endBlendNum)
{
	if (m_TargetAnimId == _id)return;

	m_IsAnimationBlending = true;
	m_TargetAnimationFrame = 0.0f;
	m_PerBlend = 0.0f;
	m_PerBlendEnd = _endBlendNum;
	m_TargetAnimId = (_id >= m_pScene->mNumAnimations) ? 0 : _id;
}



void CSkinModel::aiVector3D_Lerp(aiVector3D& _blendVec, const aiVector3D _vec1, const aiVector3D _vec2, float _blend)
{
	XMVECTOR startPos, endPos, currentPos;
	startPos = XMLoadFloat3(&XMFLOAT3(_vec1.x, _vec1.y, _vec1.z));
	endPos = XMLoadFloat3(&XMFLOAT3(_vec2.x, _vec2.y, _vec2.z));
	currentPos = XMVectorLerp(startPos, endPos, _blend);
	XMFLOAT3 vec3dCurrent;
	XMStoreFloat3(&vec3dCurrent, currentPos);
	_blendVec = aiVector3D(vec3dCurrent.x, vec3dCurrent.y, vec3dCurrent.z);
}


//************************************************
// �R���o�[�g�iaiMatrix4x4�@-> XMFLOAT4X4�j
//************************************************
XMFLOAT4X4 CSkinModel::LoadAiMatrix4x4(aiMatrix4x4* _martrix_ai)
{
	aiMatrix4x4 ai_matrix = *_martrix_ai;
	ai_matrix.Transpose();

	XMFLOAT4X4 convMatrix(
		ai_matrix.a1, ai_matrix.a2, ai_matrix.a3, ai_matrix.a4,
		ai_matrix.b1, ai_matrix.b2, ai_matrix.b3, ai_matrix.b4,
		ai_matrix.c1, ai_matrix.c2, ai_matrix.c3, ai_matrix.c4,
		ai_matrix.d1, ai_matrix.d2, ai_matrix.d3, ai_matrix.d4
		);

	return convMatrix;
}


//************************************************
// �R���o�[�g�iXMFLOAT4X4�@-> aiMatrix4x4�j
//************************************************
void CSkinModel::StoreAiMatrix4x4(XMFLOAT4X4* _matrix, aiMatrix4x4& _matrix_ai)
{
	XMFLOAT4X4 mtx = *_matrix;

	_matrix_ai =
	{
		mtx._11, mtx._12, mtx._13, mtx._14,
		mtx._21, mtx._22, mtx._23, mtx._24,
		mtx._31, mtx._32, mtx._33, mtx._34,
		mtx._41, mtx._42, mtx._43, mtx._44
	};
}

FileType CSkinModel::ChackFileType(std::string pFileType)
{
	if (pFileType.compare("fbx") == 0 || pFileType.compare("FBX") == 0)	return E_FBX;
	else if (pFileType.compare("obj") == 0 || pFileType.compare("OBJ") == 0)	return E_OBJ;

	else return E_NONE;
}

void CSkinModel::WritteName(aiNode* pNode, ofstream* fileName)
{
	if (pNode->mParent != nullptr) {
		*fileName << "(�e)" << pNode->mParent->mName.data << " => " << pNode->mName.data << "\n";
	}
	else {
		*fileName << pNode->mName.data << "\n";
	}

	// �q�m�[�h��`��
	for (int child = 0; child < pNode->mNumChildren; child++) {
		WritteName(pNode->mChildren[child], fileName);
	}
}


// World�s�񂩂畽�s�ړ������������o
XMMATRIX CSkinModel::PickupOffset(const XMMATRIX& mWorld)
{
	return XMMatrixTranslation(mWorld.r[3].m128_f32[0], mWorld.r[3].m128_f32[1], mWorld.r[3].m128_f32[2]);
}

// World�s�񂩂�g�k�����������o
XMMATRIX CSkinModel::PickupScaling(const XMMATRIX& mWorld)
{
	return XMMatrixScaling(
		XMVector3Length(XMVECTOR{ mWorld.r[0].m128_f32[0],mWorld.r[0].m128_f32[1],mWorld.r[0].m128_f32[2] }).m128_f32[0],
		XMVector3Length(XMVECTOR{ mWorld.r[1].m128_f32[0],mWorld.r[1].m128_f32[1],mWorld.r[1].m128_f32[2] }).m128_f32[0],
		XMVector3Length(XMVECTOR{ mWorld.r[2].m128_f32[0],mWorld.r[2].m128_f32[1],mWorld.r[2].m128_f32[2] }).m128_f32[0]
	);
}

// ���[���h�s�񂩂��]�����݂̂𒊏o����
XMMATRIX CSkinModel::PickupRotation(const XMMATRIX& mWorld)
{
	XMMATRIX mOffset = PickupOffset(mWorld);
	XMMATRIX mScaling = PickupScaling(mWorld);

	XMVECTOR det;
	// ������Scaling�A�E����Offset�̋t�s������ꂼ�ꂩ����B
	return XMMatrixInverse(&det, mScaling) * mWorld * XMMatrixInverse(&det, mOffset);
}


