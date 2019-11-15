#include "main.h"
#include "renderer.h"
#include "input.h"
#include "texture.h"
#include "gameObject.h"
#include <vector>

#include "modelAnimation.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imguiManager.h"

void CModelAnimation::Load(const char* file)
{
	// モデル読み込み
	m_Scene[0] = aiImportFile("asset/model/KeyframeAnimation_Idle.fbx", aiProcessPreset_TargetRealtime_MaxQuality);
	m_Scene[1] = aiImportFile("asset/model/KeyframeAnimation_Walk.fbx", aiProcessPreset_TargetRealtime_MaxQuality);
	m_Scene[2] = aiImportFile("asset/model/KeyframeAnimation_Jump.fbx", aiProcessPreset_TargetRealtime_MaxQuality);
	m_MeshNum = m_Scene[0]->mNumMeshes;
	m_Mesh = new MESH[m_MeshNum];

	m_Blend = 0.0f;
	m_CurrentAnimIndex = 0;
	m_PastAnimIndex = 0;


	// アングル初期化
	SetAngle(m_Scene[0]->mRootNode);

	// モデル情報格納
	for (int m = 0; m < m_MeshNum; m++) {

		aiMesh* mesh = m_Scene[0]->mMeshes[m];
		m_Mesh[m].IndexNum = mesh->mNumFaces * 3;

		VERTEX_3D* vertex = new VERTEX_3D[mesh->mNumVertices];

		// Vertex情報格納
		for (int i = 0; i < mesh->mNumVertices; i++) {
			vertex[i].Position = XMFLOAT3(-mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
			vertex[i].Normal = XMFLOAT3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
			vertex[i].TexCoord = XMFLOAT2(1.0f, 0.0f);

			const aiMaterial* pMat = m_Scene[0]->mMaterials[mesh->mMaterialIndex];
			aiColor4D diffuse;
			aiGetMaterialColor(pMat, AI_MATKEY_COLOR_DIFFUSE, &diffuse);	// DiffuseColor取得

			vertex[i].Diffuse = XMFLOAT4(diffuse.r * 2.0f, diffuse.g * 2.0f, diffuse.b * 2.0f, 1.0f);
		}

		// Index情報格納
		unsigned short* index;
		index = new unsigned short[mesh->mNumFaces * 3];
		for (int i = 0; i < mesh->mNumFaces; i++) {
			index[i * 3 + 0] = mesh->mFaces[i].mIndices[0];
			index[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
			index[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
		}

		// VertexBuffer 作成
		{
			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(VERTEX_3D) * mesh->mNumVertices;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = vertex;
			CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_Mesh[m].VertexBuffer);

		}

		// IndexBuffer 作成
		{
			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;
			bd.ByteWidth = sizeof(unsigned short) * m_Mesh[m].IndexNum;
			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = vertex;
			sd.pSysMem = index;
			CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_Mesh[m].IndexBuffer);

			delete[] index;
		}
			delete[] vertex;
	}
}

void CModelAnimation::Draw(XMMATRIX* world)
{
	DrawMesh(m_Scene[0]->mRootNode, world);

	DrawGUI();
}

void CModelAnimation::DrawMesh(aiNode* node, XMMATRIX* Matrix)
{
	XMMATRIX world;

	// アニメーション行列作成
	if (m_Scene[0]->mAnimations[0]->mNumChannels) {

		// 回転行列作成
		aiQuaternion aiQuat = m_NodeRotation[node->mName.C_Str()];
		XMVECTOR	 quat = XMLoadFloat4(&XMFLOAT4(aiQuat.x, aiQuat.y, aiQuat.z, aiQuat.w));
		world = XMMatrixRotationQuaternion(quat);

		//// 平行移動行列
		aiVector3D aiPos = m_NodePosition[node->mName.C_Str()];
		world *= XMMatrixTranslation(aiPos.x, aiPos.y, aiPos.z);
		//world *= XMMatrixTranspose(aiPos.x, aiPos.y, aiPos.z);
	}
	// アニメーションがない
	else {
		aiMatrix4x4 matrix = node->mTransformation;
		aiTransposeMatrix4(&matrix);
		world = XMMatrixRotationY(m_Animation[node->mName.C_Str()].Rotation.y);
		world *= XMLoadFloat4x4((XMFLOAT4X4*)& matrix);
	}

	//aiMatrix4x4 matrix = node->mTransformation;
	//aiTransposeMatrix4(&matrix);
	//world = XMMatrixRotationY(m_Animation[node->mName.C_Str()].Rotation.y);
	//world *= XMLoadFloat4x4((XMFLOAT4X4*)& matrix);

	world *= *Matrix;
	CRenderer::SetWorldMatrix(&world);

	// 自分描画
	for (int n = 0; n < node->mNumMeshes; n++) {
		unsigned int m = node->mMeshes[n];
		CRenderer::SetVertexBuffers(m_Mesh[node->mMeshes[n]].VertexBuffer);
		CRenderer::SetIndexBuffer(m_Mesh[node->mMeshes[n]].IndexBuffer);
		CRenderer::DrawIndexed(m_Mesh[node->mMeshes[n]].IndexNum, 0, 0);
	}

	// 子描画
	for (int i = 0; i < node->mNumChildren; i++) {
		DrawMesh(node->mChildren[i], &world);
	}
}

void CModelAnimation::DrawGUI()
{
	ImGui::Begin("Animation");
	if (ImGui::ArrowButton("##l", ImGuiDir_Left))
	{
		m_CurrentAnimIndex--;
		if (m_CurrentAnimIndex < 0) {
			m_CurrentAnimIndex = 2;
		}
	}
	ImGui::SameLine(0, 1.0f);
	if (ImGui::ArrowButton("##r", ImGuiDir_Right))
	{
		m_CurrentAnimIndex++;
		if (m_CurrentAnimIndex > 2) {
			m_CurrentAnimIndex = 0;
		}
	}
	ImGui::SameLine(0, 5.0f);
	ImGui::Text("AnimationIndex");
	ImGui::Checkbox("Stop", &m_Stop);
	ImGui::SliderInt("KeyFrame", &m_Frame, 0, 100);
	ImGui::Text("Blend   : %.5f", m_Blend);
	ImGui::End();
}

void CModelAnimation::Update(int frame)
{
	if (m_Scene[m_CurrentAnimIndex]->mAnimations[0]->mNumChannels) {

		aiAnimation* animation = m_Scene[m_CurrentAnimIndex]->mAnimations[0];
		for (int c = 0; c < animation->mNumChannels; c++) {

			aiNodeAnim* nodeAnim = animation->mChannels[c];

			if (m_Stop) {
				// 回転角度取得
				int CurrentFrame = (int)animation->mDuration * (m_Frame / 100.0f);

				int f = (int)CurrentFrame % nodeAnim->mNumRotationKeys;
				m_NodeRotation[nodeAnim->mNodeName.C_Str()] = nodeAnim->mRotationKeys[f].mValue;

				// 座標取得
				f = (int)CurrentFrame % nodeAnim->mNumPositionKeys;
				m_NodePosition[nodeAnim->mNodeName.C_Str()] = nodeAnim->mPositionKeys[f].mValue;
			}
			else {
				// 回転角度取得
				int f = frame % nodeAnim->mNumRotationKeys;
				m_NodeRotation[nodeAnim->mNodeName.C_Str()] = nodeAnim->mRotationKeys[f].mValue;

				// 座標取得
				f = frame % nodeAnim->mNumPositionKeys;
				m_NodePosition[nodeAnim->mNodeName.C_Str()] = nodeAnim->mPositionKeys[f].mValue;
			}
		}
	}
	else {
		PlayAnimation(m_Scene[m_CurrentAnimIndex]->mRootNode);
	}
	//PlayAnimation(m_Scene->mRootNode);

}

//void CModelAnimation::Update(int frame)
//{
//	if (m_Blend >= 1.0f) {
//		m_Blend = 1.0f;
//		m_PastAnimIndex = m_CurrentAnimIndex;
//	}
//
//	aiAnimation* anim1 = m_Scene[m_PastAnimIndex]->mAnimations[0];
//	aiAnimation* anim2 = m_Scene[m_CurrentAnimIndex]->mAnimations[0];
//
//	for (int c = 0; c < anim1->mNumChannels; c++) {
//
//		aiNodeAnim* nodeAnim1 = anim1->mChannels[c];
//		aiNodeAnim* nodeAnim2 = anim2->mChannels[c];
//
//		aiQuaternion quaternion;
//		aiQuaternion::Interpolate(quaternion, nodeAnim1->mRotationKeys[frame % nodeAnim1->mNumRotationKeys].mValue, nodeAnim2->mRotationKeys[frame % nodeAnim2->mNumRotationKeys].mValue, m_Blend);
//		m_NodeRotation[nodeAnim1->mNodeName.C_Str()] = quaternion;
//
//		XMVECTOR startPos, endPos, currentPos;
//		startPos = XMLoadFloat3(&XMFLOAT3(nodeAnim1->mPositionKeys[frame % nodeAnim1->mNumPositionKeys].mValue.x, nodeAnim1->mPositionKeys[frame % nodeAnim1->mNumPositionKeys].mValue.y, nodeAnim1->mPositionKeys[frame % nodeAnim1->mNumPositionKeys].mValue.z));
//		endPos = XMLoadFloat3(&XMFLOAT3(nodeAnim2->mPositionKeys[frame % nodeAnim2->mNumPositionKeys].mValue.x, nodeAnim2->mPositionKeys[frame % nodeAnim2->mNumPositionKeys].mValue.y, nodeAnim2->mPositionKeys[frame % nodeAnim2->mNumPositionKeys].mValue.z));
//		currentPos = XMVectorLerp(startPos, endPos, m_Blend);
//		XMFLOAT3 vec3dCurrent;
//		XMStoreFloat3(&vec3dCurrent, currentPos);
//		aiVector3D aiVec3d = aiVector3D(vec3dCurrent.x, vec3dCurrent.y, vec3dCurrent.z);
//		m_NodePosition[nodeAnim1->mNodeName.C_Str()] = aiVec3d;
//	}
//
//	m_Blend += 0.001f;
//}

void CModelAnimation::PlayAnimation(aiNode* pNode)
{
	std::string comp = pNode->mName.C_Str();

	// 自分のアニメーション更新
	if (CInput::GetKeyPress('J')) {

		if (!comp.compare("LeftShoulder")) {

			m_Animation[pNode->mName.C_Str()].Rotation.x += 0.0f;
			m_Animation[pNode->mName.C_Str()].Rotation.y += 0.03f;
			m_Animation[pNode->mName.C_Str()].Rotation.z += 0.0f;
		}
	}

	// 子階層のアニメーション更新
	for (int i = 0; i < pNode->mNumChildren; i++) {
		PlayAnimation(pNode->mChildren[i]);
	}
}

void CModelAnimation::SetAngle(aiNode* pNode)
{
	m_Animation[pNode->mName.C_Str()].Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < pNode->mNumChildren; i++) {
		SetAngle(pNode->mChildren[i]);
	}
}

void CModelAnimation::Unload()
{
	for (int m = m_MeshNum - 1; m >= 0; m--) {

		m_Mesh[m].VertexBuffer->Release();
		m_Mesh[m].IndexBuffer->Release();
	}

	delete [] m_Mesh;
	aiReleaseImport(m_Scene[2]);
	aiReleaseImport(m_Scene[1]);
	aiReleaseImport(m_Scene[0]);
}

aiVector3D CModelAnimation::aiVectorLerp(aiVector3D v1, aiVector3D v2, float blend)
{
	aiVector3D position;

	position.x = (1.0f - blend) * v1.x + blend * v2.x;
	position.y = (1.0f - blend) * v1.y + blend * v2.y;
	position.z = (1.0f - blend) * v1.z + blend * v2.z;

	return position;
}
