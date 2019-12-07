// ライブラリ
#pragma comment(lib, "assimp.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

// インクルード
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

#define F "asset/Node.txt"


/* Assimpの行列の合成順番は違うから気を付けよう！！
	 A *= B;
	 本来は : A = A * B;
	 Assimpは : A = B * A;
*/


static ofstream outputfile(F);


//************************************************
// ファイルロード
//************************************************
void CSkinModel::Load(char* pFileName, float size)
{
	m_pScene = aiImportFile(pFileName, aiProcessPreset_TargetRealtime_MaxQuality);
	m_pScene->mMeshes[0]->mBones[0];
	if (m_pScene == NULL)
	{
		assert(false);
	}

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


	WritteName(m_pScene->mRootNode);
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

	m_Shader = ShaderManager::GetShader<CShaderDefault>();
}


//************************************************
// メッシュごとにバッファ作成
//************************************************
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
				vertices[id].Normal = Vector3(pVertex->Normal.x, pVertex->Normal.y, pVertex->Normal.z);
				vertices[id].Position = Vector3(pVertex->Position.x, pVertex->Position.y, pVertex->Position.z);
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


//************************************************
// ファイルアンロード
//************************************************
void CSkinModel::Unload()
{
	aiReleaseImport(m_pScene);
}


//************************************************
// 描画
//************************************************
void CSkinModel::Draw(XMMATRIX* world)
{
	XMMATRIX _world = *world;

	// XMMATRIX行列の合成
	_world = XMMatrixScaling(m_Size, m_Size, m_Size) * _world;

	XMFLOAT4X4 world_4x4;
	XMStoreFloat4x4(&world_4x4, _world);

	CCamera* camera = CCameraManager::GetCamera();

	m_Shader->SetWorldMatrix(&world_4x4);
	m_Shader->SetViewMatrix(&camera->GetViewMatrix());
	m_Shader->SetProjectionMatrix(&camera->GetProjectionMatrix());
	m_Shader->SetLight(LIGHT());

	//CRenderer::SetWorldMatrix(&_world);

	if (m_DrawAtLine) {
		CRenderer::SetRasterizerState(D3D11_FILL_WIREFRAME, D3D11_CULL_NONE);
	}
	else {
		CRenderer::SetRasterizerState(D3D11_FILL_SOLID, D3D11_CULL_NONE);
	}
	// 描画
	DrawMesh(m_pScene->mRootNode);

	CRenderer::SetRasterizerState(D3D11_FILL_SOLID, D3D11_CULL_NONE);
}


//************************************************
// メッシュごとの描画
//************************************************
void CSkinModel::DrawMesh(const aiNode* pNode)
{
	for (int mesh = 0; mesh < pNode->mNumMeshes; mesh++) {

		// メッシュ1つ分取得
		const aiMesh* pMesh = m_pScene->mMeshes[pNode->mMeshes[mesh]];
		unsigned int mesh_index = pNode->mMeshes[mesh];
		m_Mesh[mesh_index].IndexNum = pMesh->mNumFaces * 3;

		// メッシュごとの頂点群を取得
		VERTEX_3D* vertices = nullptr;
		vertices = new VERTEX_3D[pMesh->mNumVertices];
		assert(vertices);

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
				vertices[id].Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
				vertices[id].Normal   = Vector3(pVertex->DeformNormal.x, pVertex->DeformNormal.y, pVertex->DeformNormal.z);
				vertices[id].Position = Vector3(pVertex->DeformPosition.x, pVertex->DeformPosition.y, pVertex->DeformPosition.z);
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
		CRenderer::SetTexture(m_Texture[pNode->mName.C_Str()]);

		{
			// マテリアル取得
			const aiMaterial* mat = m_pScene->mMaterials[pMesh->mMaterialIndex];
			assert(mat);
			aiColor4D diffuse, ambient;
			aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &diffuse);	// ディフーズカラー取得
			aiGetMaterialColor(mat, AI_MATKEY_COLOR_AMBIENT, &ambient);	// ディフーズカラー取得

			// テクスチャ取得
			aiString path;
			mat->GetTexture(aiTextureType_DIFFUSE, 0, &path);

			MATERIAL material;
			material.Ambient = COLOR(ambient.r, ambient.g, ambient.b, ambient.a);
			material.Diffuse = COLOR(diffuse.r, diffuse.g, diffuse.b, diffuse.a);

			m_Shader->SetMaterial(material);
			m_Shader->Set();
		}

		{
			UINT Stride = sizeof(VERTEX_3D);
			UINT offdet = 0;
			CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_Mesh[mesh_index].VertexBuffer, &Stride, &offdet);	// バーテクスバッファセット
			CRenderer::GetDeviceContext()->IASetIndexBuffer(m_Mesh[mesh_index].IndexBuffer, DXGI_FORMAT_R16_UINT, 0);		// インデックスバッファセット
		}

		CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		CRenderer::DrawIndexed( m_Mesh[mesh_index].IndexNum, 0, 0 );
		//CRenderer::GetDeviceContext()->DrawIndexedInstanced(m_Mesh[mesh_index].IndexNum, 100, 0, 0, 0);
	}


	// 子描画
	for (int i = 0; i < pNode->mNumChildren; i++) {
		DrawMesh(pNode->mChildren[i]);
	}
}


//************************************************
// ボーンの初期化
//************************************************
void CSkinModel::CreateBone(aiNode* pNode)
{
	BONE bone;
	m_Bone[pNode->mName.C_Str()] = bone; // 何も初期化されていないゴミデータを入れることで"器"を作っておく
	m_Texture[pNode->mName.C_Str()] = new CTexture();
	m_Texture[pNode->mName.C_Str()]->LoadSTB("asset/image/white.png");

	for (int i = 0; i < pNode->mNumChildren; i++) {
		CreateBone(pNode->mChildren[i]);
	}
}


//************************************************
// アニメーションデータ更新
//************************************************
void CSkinModel::update(int addAnimationFrame)
{
	// アニメーションデータを持っているか
	if (!m_pScene->HasAnimations()) return;

	// アニメーションキー更新
	m_AnimationFrame += addAnimationFrame * m_AnimationSpeed;

	// アニメーションブレンドするか？しないか？
	m_IsAnimationBlending ? AnimationBlend() : Animation();

	// ボーンの各行列を計算
	CalculateBoneMatrix();

	// 再帰的にボーンデータ更新
	UpdateBoneMatrix(m_pScene->mRootNode, aiMatrix4x4());
}


void CSkinModel::CalculateBoneMatrix()
{
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


void CSkinModel::AnimationBlend()
{
	// アニメーションデータ取得									↓どのアニメ―ション？
	aiAnimation* pAnimationCurrent = m_pScene->mAnimations[m_CurrentAnimId];
	aiAnimation* pAnimationTarget = m_pScene->mAnimations[m_TargetAnimId];


	for (auto c = 0; c < pAnimationCurrent->mNumChannels; c++) {

		// 現在フレームのアニメーション行列の回転・平行移動成分を取得
		int CurrentFrame = m_IsStopMotion ? (int)pAnimationCurrent->mDuration * (m_MotionFrame / 100.0f) : static_cast<int>(m_AnimationFrame);

		// ノードアニメーション取得
		aiNodeAnim* pNodeAnimCurrent = pAnimationCurrent->mChannels[c];
		aiNodeAnim* pNodeAnimTarget = pAnimationTarget->mChannels[c];
		BONE* pBone = &m_Bone[pNodeAnimCurrent->mNodeName.C_Str()];

		aiQuaternion rotCurrent, rotTarget, rotBlend;
		aiVector3D posCurrent, posTarget, posBlend;

		rotCurrent = pNodeAnimCurrent->mRotationKeys[CurrentFrame % pNodeAnimCurrent->mNumRotationKeys].mValue;
		posCurrent = pNodeAnimCurrent->mPositionKeys[CurrentFrame % pNodeAnimCurrent->mNumPositionKeys].mValue;
		rotTarget = pNodeAnimTarget->mRotationKeys[CurrentFrame % pNodeAnimTarget->mNumRotationKeys].mValue;
		posTarget = pNodeAnimTarget->mPositionKeys[CurrentFrame % pNodeAnimTarget->mNumPositionKeys].mValue;


		// 回転をブレンド
		aiQuaternion::Interpolate(rotBlend, rotCurrent, rotTarget, m_PerBlend);

		// ポジションをブレンド
		aiVector3D_Lerp(posBlend, posCurrent, posTarget, m_PerBlend);

		// 行列にしてクォータニオンとして格納
		pBone->AnimationMatrix = aiMatrix4x4(
			aiVector3D(1.0f, 1.0f, 1.0f), // 拡大縮小
			rotBlend,					  // 回転
			posBlend                      // 移動
		);
	}


	// ブレンド値を更新
	m_PerBlend += 0.05;

	if (m_PerBlend >= 1.0f) {

		m_IsAnimationBlending = false;
		m_CurrentAnimId = m_TargetAnimId;
		m_PerBlend = 0.0f;
	}

}


void CSkinModel::Animation()
{
	// アニメーションデータ取得									↓どのアニメ―ション？
	aiAnimation* pAnimationCurrent = m_pScene->mAnimations[m_CurrentAnimId];


	for (auto c = 0; c < pAnimationCurrent->mNumChannels; c++) {

		// 現在フレームのアニメーション行列の回転・平行移動成分を取得
		int CurrentFrame = m_IsStopMotion ? (int)pAnimationCurrent->mDuration * (m_MotionFrame / 100.0f) : static_cast<int>(m_AnimationFrame);

		// ノードアニメーション取得
		aiNodeAnim* pNodeAnimCurrent = pAnimationCurrent->mChannels[c];
		BONE* pBone = &m_Bone[pNodeAnimCurrent->mNodeName.C_Str()];

		aiQuaternion rotCurrent;
		aiVector3D posCurrent;

		rotCurrent = pNodeAnimCurrent->mRotationKeys[CurrentFrame % pNodeAnimCurrent->mNumRotationKeys].mValue;
		posCurrent = pNodeAnimCurrent->mPositionKeys[CurrentFrame % pNodeAnimCurrent->mNumPositionKeys].mValue;

		// 行列にしてクォータニオンとして格納
		pBone->AnimationMatrix = aiMatrix4x4(
			aiVector3D(1.0f, 1.0f, 1.0f), // 拡大縮小
			rotCurrent,					  // 回転
			posCurrent                      // 移動
		);
	}
}

//************************************************
// ボーン行列更新
//************************************************
void CSkinModel::UpdateBoneMatrix(aiNode* pNode, aiMatrix4x4 matrix)
{
	BONE* pBone = &m_Bone[pNode->mName.C_Str()];

	aiMatrix4x4 worldMatrix;
	worldMatrix = matrix;
	worldMatrix *= pBone->AnimationMatrix;
	pBone->WorldMatrix = worldMatrix;
	pBone->Matrix = worldMatrix;
	pBone->Matrix *= pBone->OffsetMatrix;

	for (int n = 0; n < pNode->mNumChildren; n++) {

		UpdateBoneMatrix(pNode->mChildren[n], worldMatrix);
	}
}

//************************************************
// 名前からノードをゲット
//************************************************
aiNode* CSkinModel::GetBoneNode(aiNode* pNode, const char* _name)
{
	string thisNodeName = pNode->mName.C_Str();
	string Name = _name;

	if (thisNodeName.compare(Name) == 0) {
		return pNode;
	}

	// 子探索
	for (int i = 0; i < pNode->mNumChildren; i++) {

		aiNode* node = GetBoneNode(pNode->mChildren[i], _name);

		if (node != NULL) {

			return node;
		}
	}

	return NULL;
}


//************************************************
// 名前からワールド座標をゲット
//************************************************
static XMMATRIX g_TargetMatrix;
Vector3 CSkinModel::GetWorldPosition(XMMATRIX* _world, const char* _bone_name)
{
	g_TargetMatrix = XMMatrixIdentity();

	XMMATRIX mtxWorld = XMMatrixScaling(m_Size, m_Size, m_Size);
	mtxWorld *= *_world;
	GetPosLocalToWorld(m_pScene->mRootNode, &mtxWorld, _bone_name);
	//g_TargetMatrix = g_TargetMatrix * XMMatrixScaling(m_Size, m_Size, m_Size);
	//g_TargetMatrix *= *_world;
	XMVECTOR position = { 0.0f, 0.0f, 0.0f, 0.0f };
	position = XMVector3Transform(position, g_TargetMatrix);

	return position;
}


//************************************************
// 親との相対位置をゲット
//************************************************
void CSkinModel::GetPosLocalToWorld(aiNode* pNode, XMMATRIX* _world, const char* _targetName)
{
	// 回転成分を取得
	XMMATRIX mtxWorld = XMLoadFloat4x4(&LoadAiMatrix4x4(&m_Bone[pNode->mName.C_Str()].WorldMatrix));//PickupRotation(XMLoadFloat4x4(&LoadAiMatrix4x4(&m_Bone[pNode->mName.C_Str()].AnimationMatrix)));
	if (pNode->mParent) {
		mtxWorld *= XMMatrixInverse(NULL, XMLoadFloat4x4(&LoadAiMatrix4x4(&m_Bone[pNode->mParent->mName.C_Str()].Matrix)));
	}
	else {
		mtxWorld *= XMMatrixInverse(NULL, *_world);
	}
	mtxWorld *= *_world;

	// 対象のノードだったらターゲット行列にセット
	string name = _targetName;
	if (name.compare(pNode->mName.C_Str()) == 0) {
		g_TargetMatrix = mtxWorld;
		return;
	}

	// 子ノードにアクセス
	for (int i = 0; i < pNode->mNumChildren; i++) {
		GetPosLocalToWorld(pNode->mChildren[i], &mtxWorld, _targetName);
	}
}


void CSkinModel::SetAnimation(bool _next) {

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

void CSkinModel::SetAnimation(const unsigned int _id, const float _startBlendNum) {

	if (m_IsAnimationBlending)return;
	if (m_TargetAnimId == _id)return;

	m_IsAnimationBlending = true;

	m_PerBlend =(_startBlendNum > 1.0f) ? 1.0f : _startBlendNum;
	m_PerBlend =(_startBlendNum < 0.0f) ? 0.0f : _startBlendNum;

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
// コンバート（aiMatrix4x4　-> XMFLOAT4X4）
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
// コンバート（XMFLOAT4X4　-> aiMatrix4x4）
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

void CSkinModel::WritteName(aiNode* pNode)
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


// World行列から平行移動成分だけ抽出
XMMATRIX CSkinModel::PickupOffset(const XMMATRIX& mWorld)
{
	return XMMatrixTranslation(mWorld.r[3].m128_f32[0], mWorld.r[3].m128_f32[1], mWorld.r[3].m128_f32[2]);
}

// World行列から拡縮成分だけ抽出
XMMATRIX CSkinModel::PickupScaling(const XMMATRIX& mWorld)
{
	return XMMatrixScaling(
		XMVector3Length(XMVECTOR{ mWorld.r[0].m128_f32[0],mWorld.r[0].m128_f32[1],mWorld.r[0].m128_f32[2] }).m128_f32[0],
		XMVector3Length(XMVECTOR{ mWorld.r[1].m128_f32[0],mWorld.r[1].m128_f32[1],mWorld.r[1].m128_f32[2] }).m128_f32[0],
		XMVector3Length(XMVECTOR{ mWorld.r[2].m128_f32[0],mWorld.r[2].m128_f32[1],mWorld.r[2].m128_f32[2] }).m128_f32[0]
	);
}

// ワールド行列から回転成分のみを抽出する
XMMATRIX CSkinModel::PickupRotation(const XMMATRIX& mWorld)
{
	XMMATRIX mOffset = PickupOffset(mWorld);
	XMMATRIX mScaling = PickupScaling(mWorld);

	XMVECTOR det;
	// 左からScaling、右からOffsetの逆行列をそれぞれかける。
	return XMMatrixInverse(&det, mScaling) * mWorld * XMMatrixInverse(&det, mOffset);
}


