// ライブラリ
#pragma comment(lib, "assimp.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

// インクルード
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/matrix4x4.h>

#include <iostream>
#include <fstream>
#include <math.h>
#include <unordered_map>
#include <vector>
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "texture.h"
#include "gameObject.h"
#include "skinmesh_animation.h"
#include "modelAnimation.h"

#define F "Node.txt"


/* Assimpの行列の合成順番は違うから気を付けよう！！
	 A *= B;
	 本来は : A = A * B;
	 Assimpは : A = B * A;
*/

FileType ChackFileType(std::string pFileType);
void WritteName(aiNode* pNode);
XMFLOAT4X4 LoadAiMatrix4x4(aiMatrix4x4* _martrix_ai);
void StoreAiMatrix4x4(XMFLOAT4X4* _matrix_ai, aiMatrix4x4& _martrix_ai);

static ofstream outputfile(F);

void CSkinModel::Load(char* pFileName, float size)
{
	m_pScene = aiImportFile(pFileName, aiProcessPreset_TargetRealtime_MaxQuality);
	assert(m_pScene);

	// 各ボーン名取得
	CreateBone(m_pScene->mRootNode);

	m_Size = size;
	m_Mesh = new MESH[m_pScene->mNumMeshes];

	// 頂点初期化
	m_pDeformVertex = new std::vector<DEFORM_VERTEX>[m_pScene->mNumMeshes];
	for (int m = 0; m < m_pScene->mNumMeshes; m++) {

		aiMesh* pMesh = m_pScene->mMeshes[m];

		// 各メッシュの頂点初期化
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

			// リストに追加
			m_pDeformVertex[m].push_back(vertex);
		}

		// 各メッシュに関係したボーンを格納
		for (int b = 0; b < pMesh->mNumBones; b++) {

			//if (!pMesh->HasBones) continue;

			aiBone* pBone = pMesh->mBones[b];
			m_Bone[pBone->mName.C_Str()].OffsetMatrix = pBone->mOffsetMatrix;
			// FBXファイルはボーンが自分に属している頂点のインデックスを持っている。
			// これだとゲームでは不都合なので逆にしている。
			// 頂点が属しているボーンのインデックスを持つようにする！！

			for (int w = 0; w < pBone->mNumWeights; w++) {

				aiVertexWeight weight = pBone->mWeights[w];

				assert(m_pDeformVertex[m][weight.mVertexId].BoneNum <= 4);

				m_pDeformVertex[m][weight.mVertexId].BoneWeight[m_pDeformVertex[m][weight.mVertexId].BoneNum] = weight.mWeight;		// ボーンの動きに対する重み
				m_pDeformVertex[m][weight.mVertexId].BoneName[m_pDeformVertex[m][weight.mVertexId].BoneNum] = pBone->mName.C_Str(); // 自分が属するボーンの名前
				m_pDeformVertex[m][weight.mVertexId].BoneNum++;																		//
			}
		}
	}


	LoadMesh(m_pScene->mRootNode);


	aiNode* pNode = m_pScene->mRootNode;
	WritteName(pNode);
	outputfile.close();


	// モデルデータとテクスチャが同じファイルに入っている場合
	//m_Texture = new unsigned int[m_pScene->mNumMaterials];
	std::string modelPath = pFileName;

	size_t fileNamePos = modelPath.find_last_of(".");								// モデルのパスの後ろから最初に出てきた"."の場所を取得
	std::string fileType = modelPath.substr(fileNamePos + 1, sizeof(modelPath));	// ファイルの拡張子を取得

	m_FileType = ChackFileType(fileType);	// ファイルタイプ取得

	//// テクスチャ取得
	aiString path;

	//for (int tex = 0; tex < m_pScene->mNumMaterials; tex++) {

	//	if (m_pScene->mMaterials[tex]->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {

	//		// fbxファイルの内部にテクスチャが入っているか否か
	//		if (path.data[0] == '*') {
	//			// fbxファイル内にテクスチャあり
	//			int id = atoi(&path.data[1]);
	//			m_Texture[path.data] = new CTexture();
	//			m_Texture[path.data]->loadTextureFromMemory((const unsigned char*)m_pScene->mTextures[id]->pcData, m_pScene->mTextures[id]->mWidth);
	//		}
	//		else {
	//			// fbxファイル外にテクスチャあり
	//			//size_t pos = modelPath.find_last_of("\\/");				// モデルのパスの後ろから最初に出てきた"\\/"の場所を取得
	//			//std::string texPath = (char*)& path.data;					// Textureのパス取得
	//			//size_t texpos = texPath.find_last_of("\\/");				// Textureのパスの後ろから最初に出てきた"\\/"の場所を取得
	//			//std::string texturePath = modelPath.substr(0, pos + 1);		// モデルと同じ場所のパスを取得
	//			//texturePath += texPath.substr(texpos + 1, sizeof(texPath));	// モデルと同じ場所にあるテクスチャのパスに書き換え
	//			//m_Texture[tex] = LoadTextureSTB(texturePath.c_str());		// 指定したパスのテクスチャを読み込む
	//		}
	//	}
	//}

	//DrawMesh(m_pScene->mRootNode, &aiMatrix4x4());
}
void CSkinModel::LoadMesh(const aiNode* pNode)
{
	for (int mesh = 0; mesh < pNode->mNumMeshes; mesh++) {

		// メッシュ1つ分取得
		const aiMesh* pMesh = m_pScene->mMeshes[pNode->mMeshes[mesh]];
		unsigned int mesh_index = pNode->mMeshes[mesh];
		m_Mesh[mesh_index].IndexNum = pMesh->mNumFaces * 3;

		// マテリアル取得
		const aiMaterial* mat = m_pScene->mMaterials[pMesh->mMaterialIndex];
		aiColor4D diffuse;
		aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &diffuse);	// ディフーズカラー取得

		VERTEX_3D* vertices = new VERTEX_3D[pMesh->mNumVertices];

		// Index情報格納
		unsigned short* Indices;
		Indices = new unsigned short[pMesh->mNumFaces * 3];

		// メッシュの面ポリゴンの個数分繰り返し
		for (int face = 0; face < pMesh->mNumFaces; face++) {
			const aiFace* pFace = &pMesh->mFaces[face];	// 面ポリゴン一個分のデータ取得

			assert(pFace->mNumIndices == 3);

			Indices[face * 3 + 0] = pFace->mIndices[0];
			Indices[face * 3 + 1] = pFace->mIndices[1];
			Indices[face * 3 + 2] = pFace->mIndices[2];

			std::vector<DEFORM_VERTEX>* pVertices = &m_pDeformVertex[mesh_index];

			// 1つの面ポリゴンの頂点数を取得
			for (int vertex = 0; vertex < pFace->mNumIndices; vertex++) {

				// 各頂点毎にアクセス
				int id = pFace->mIndices[vertex];
				DEFORM_VERTEX* pVertex = &(*pVertices)[id];

				assert(pVertex);

				// 頂点情報格納
				vertices[id].Diffuse = XMFLOAT4(diffuse.r, diffuse.g, diffuse.b, 1.0f);
				vertices[id].Normal = XMFLOAT3(pVertex->Normal.x, pVertex->Normal.y, pVertex->Normal.z);
				vertices[id].Position = XMFLOAT3(pVertex->Position.x, pVertex->Position.y, pVertex->Position.z);
				vertices[id].TexCoord = pMesh->HasTextureCoords(0) ? XMFLOAT2(pMesh->mTextureCoords[0][id].x, 1.0f - pMesh->mTextureCoords[0][id].y) : XMFLOAT2(0.0f, 0.0f);
			}
		}

		// VertexBuffer 作成
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

		// IndexBuffer 作成
		{
			D3D11_BUFFER_DESC bd{};
			bd.ByteWidth = sizeof(unsigned short) * m_Mesh[mesh_index].IndexNum;
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

void CSkinModel::Unload()
{
	aiReleaseImport(m_pScene);
}

void CSkinModel::Draw(XMMATRIX* world)
{
	XMMATRIX _world = *world;

	// XMMATRIX行列の合成
	_world = XMMatrixScaling(m_Size, m_Size, m_Size) * _world;

	CRenderer::SetWorldMatrix(&_world);

	// 描画
	DrawMesh(m_pScene->mRootNode);
}

void CSkinModel::DrawMesh(const aiNode* pNode)
{
	for (int mesh = 0; mesh < pNode->mNumMeshes; mesh++) {

		// メッシュ1つ分取得
		const aiMesh* pMesh = m_pScene->mMeshes[pNode->mMeshes[mesh]];
		unsigned int mesh_index = pNode->mMeshes[mesh];
		m_Mesh[mesh_index].IndexNum = pMesh->mNumFaces * 3;

		// マテリアル取得
		const aiMaterial* mat = m_pScene->mMaterials[pMesh->mMaterialIndex];
		aiColor4D diffuse;
		aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &diffuse);	// ディフーズカラー取得

		// テクスチャ取得
		aiString path;
		mat->GetTexture(aiTextureType_DIFFUSE, 0, &path);

		VERTEX_3D* vertices = new VERTEX_3D[pMesh->mNumVertices];

		// メッシュの面ポリゴンの個数分繰り返し
		for (int face = 0; face < pMesh->mNumFaces; face++) {

			const aiFace* pFace = &pMesh->mFaces[face];	// 面ポリゴン一個分のデータ取得

			assert(pFace->mNumIndices == 3);

			std::vector<DEFORM_VERTEX>* pVertices = &m_pDeformVertex[mesh_index];

			// 1つの面ポリゴンの頂点数を取得
			for (int vertex_id = 0; vertex_id < pFace->mNumIndices; vertex_id++) {

				// 各頂点毎にアクセス
				int id = pFace->mIndices[vertex_id];
				DEFORM_VERTEX* pVertex = &(*pVertices)[id];

				assert(pVertex);

				// 頂点情報格納
				vertices[id].Diffuse  = XMFLOAT4(diffuse.r, diffuse.g, diffuse.b, 1.0f);
				vertices[id].Normal   = XMFLOAT3(pVertex->DeformNormal.x, pVertex->DeformNormal.y, pVertex->DeformNormal.z);
				vertices[id].Position = XMFLOAT3(pVertex->DeformPosition.x, pVertex->DeformPosition.y, pVertex->DeformPosition.z);
				vertices[id].TexCoord = pMesh->HasTextureCoords(0) ? XMFLOAT2(pMesh->mTextureCoords[0][id].x, 1.0f - pMesh->mTextureCoords[0][id].y) : XMFLOAT2(0.0f, 0.0f);
			}
		}


		// VertexBuffer書き換え
		{
			D3D11_MAPPED_SUBRESOURCE msr;
			CRenderer::GetDeviceContext()->Map(m_Mesh[mesh_index].VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
			memcpy(msr.pData, vertices, sizeof(VERTEX_3D) * pMesh->mNumVertices);
			CRenderer::GetDeviceContext()->Unmap(m_Mesh[mesh_index].VertexBuffer, 0);
		}

		delete[] vertices;


		//// テクスチャ読み込み //////
		//assert(m_Texture[pNode->mName.C_Str()]);
		//m_Texture[pNode->mName.C_Str()]->LoadSTB(path.C_Str());
		//CRenderer::SetTexture(m_Texture[pNode->mName.C_Str()]);

		CRenderer::SetVertexBuffers(m_Mesh[mesh_index].VertexBuffer);
		CRenderer::SetIndexBuffer(m_Mesh[mesh_index].IndexBuffer);
		CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		CRenderer::DrawIndexed(m_Mesh[mesh_index].IndexNum, 0, 0);
	}


	// 子描画
	for (int i = 0; i < pNode->mNumChildren; i++) {
		DrawMesh(pNode->mChildren[i]);
	}
}

void CSkinModel::CreateBone(aiNode* pNode)
{
	BONE bone;
	m_Bone[pNode->mName.C_Str()] = bone; // 何も初期化されていないゴミデータを入れることで"器"を作っておく
	m_Texture[pNode->mName.C_Str()] = new CTexture();

	for (int i = 0; i < pNode->mNumChildren; i++) {
		CreateBone(pNode->mChildren[i]);
	}
}

void CSkinModel::Animation(int frame)
{
	// アニメーションデータを持っているか
	if (m_pScene->HasAnimations()) {

		// アニメーションデータ取得                     ↓どのアニメ―ション？
		aiAnimation* pAnimation = m_pScene->mAnimations[m_CurrentAnimId];

		for (auto c = 0; c < pAnimation->mNumChannels; c++) {

			// ノードアニメーション取得
			aiNodeAnim* pNodeAnim = pAnimation->mChannels[c];
			BONE* pBone = &m_Bone[pNodeAnim->mNodeName.C_Str()];

			aiQuaternion rot;
			aiVector3D pos;

			// 現在フレームのアニメーション行列の回転・平行移動成分を取得
			int CurrentFrame = m_IsStopMotion ? (int)pAnimation->mDuration * (m_MotionFrame / 100.0f) : frame;
			rot = pNodeAnim->mRotationKeys[CurrentFrame % pNodeAnim->mNumRotationKeys].mValue;
			pos = pNodeAnim->mPositionKeys[CurrentFrame % pNodeAnim->mNumPositionKeys].mValue;

			// 行列にしてクォータニオンとして格納
			pBone->AnimationMatrix = aiMatrix4x4(
				aiVector3D(1.0f, 1.0f, 1.0f), // 拡大縮小
				rot,						  // 回転
				pos                           // 移動
			);


			// 再帰的にボーンデータ更新
			UpdateBoneMatrix(m_pScene->mRootNode, aiMatrix4x4());

			// 各頂点の座標計算（本来ならシェーダーがやるべき）
			/*
			  ある頂点を変換する行列は =
			  A * aw + B * bw + C * cw + D * dw		(aw + bw + cw + dw == 1.0f)

			  ( 大文字 : 関連するボーン, 小文字 : そのウェイト )
			*/
			for (unsigned int m = 0; m < m_pScene->mNumMeshes; m++)
			{
				for (auto& vertex : m_pDeformVertex[m])
				{
					aiMatrix4x4 matrix[4];
					aiMatrix4x4 outMatrix;
					matrix[0] = m_Bone[vertex.BoneName[0]].Matrix;
					matrix[1] = m_Bone[vertex.BoneName[1]].Matrix;
					matrix[2] = m_Bone[vertex.BoneName[2]].Matrix;
					matrix[3] = m_Bone[vertex.BoneName[3]].Matrix;

					//ウェイトを考慮してマトリクス算出
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


					//法線変換用に移動成分を削除
					outMatrix.a4 = 0.0f;
					outMatrix.b4 = 0.0f;
					outMatrix.c4 = 0.0f;

					vertex.DeformNormal = vertex.Normal;
					vertex.DeformNormal *= outMatrix;
				}
			}
		}
	}

}

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

XMFLOAT4X4 LoadAiMatrix4x4(aiMatrix4x4* _martrix_ai)
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

void StoreAiMatrix4x4(XMFLOAT4X4* _matrix_ai, aiMatrix4x4& _martrix_ai)
{
	XMFLOAT4X4 mtx = *_matrix_ai;

	_martrix_ai =
	{
		mtx._11, mtx._12, mtx._13, mtx._14,
		mtx._21, mtx._22, mtx._23, mtx._24,
		mtx._31, mtx._32, mtx._33, mtx._34,
		mtx._41, mtx._42, mtx._43, mtx._44
	};
}

FileType ChackFileType(std::string pFileType)
{
	if (pFileType.compare("fbx") == 0 || pFileType.compare("FBX") == 0)	return E_FBX;
	else if (pFileType.compare("obj") == 0 || pFileType.compare("OBJ") == 0)	return E_OBJ;

	else return E_NONE;
}

void WritteName(aiNode* pNode)
{
	if (pNode->mParent != nullptr) {
		outputfile << "(親)" << pNode->mParent->mName.data << " => " << pNode->mName.data << "\n";
	}
	else {
		outputfile << pNode->mName.data << "\n";
	}

	// 子ノードを描画
	for (int child = 0; child < pNode->mNumChildren; child++) {
		WritteName(pNode->mChildren[child]);
	}
}

