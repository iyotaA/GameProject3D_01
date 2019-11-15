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
#include "skinmesh.h"

//bool CSkinMesh::LoadMesh(const string& pFileName)
//{
//	bool Ret = false;
//
//	m_pScene = aiImportFile(pFileName.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
//	assert(m_pScene);
//
//	m_GlobalInverseTransform = m_pScene->mRootNode->mTransformation;
//	m_GlobalInverseTransform.Inverse();
//	Ret = InitFromScene(m_pScene, pFileName);
//
//	// Make sure the VAO is not changed from the outside
//	glBindVertexArray(0);
//
//	return Ret;
//}
//
//bool CSkinMesh::InitFromScene(const aiScene* pScene, const string& Filename)
//{
//	vector<VertexBoneData> Bones;
//	Bones.resize(NumVertices);
//	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[BONE_VB]);
//
//	glBufferData(GL_ARRAY_BUFFER, sizeof(Bones[0]) * Bones.size(), &Bones[0], GL_STATIC_DRAW);
//	glEnableVertexAttribArray(BONE_ID_LOCATION);
//	glVertexAttribIPointer(BONE_ID_LOCATION, 4, GL_INT, sizeof(VertexBoneData), (const GLvoid*)0);
//	glEnableVertexAttribArray(BONE_WEIGHT_LOCATION);
//	glVertexAttribPointer(BONE_WEIGHT_LOCATION, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)16);
//}