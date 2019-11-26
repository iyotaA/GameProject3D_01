#ifndef MODEL_ANIMATION_H_
#define MODEL_ANIMATION_H_

// ライブラリ
#pragma comment(lib, "assimp.lib")

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/matrix4x4.h>

#include <iostream>
#include <unordered_map>
using namespace std;


struct MESH
{
	ID3D11Buffer* VertexBuffer;
	ID3D11Buffer* IndexBuffer;
	unsigned int  IndexNum;
	MATERIAL*	  Material;
};

struct ANIMATION
{
	XMFLOAT3 Rotation;
};

class CModelAnimation
{
public:
	void Draw(XMMATRIX* pMtx);
	void DrawMesh(aiNode* node, XMMATRIX* pMtx);
	void DrawGUI();
	void Load(const char* FileName);
	void Update(int frame);
	void PlayAnimation(aiNode* pNode);
	void Unload();
	void SetAngle(aiNode* pNode);
	void SetAnimationIndex(int currentIndex) {
		m_PastAnimIndex = m_CurrentAnimIndex;
		m_CurrentAnimIndex = currentIndex;
		m_Blend = 0.0f;
	};

	XMFLOAT3* GetNodePosition(char* pNodeName){
		XMFLOAT3 pos = XMFLOAT3(m_NodePosition[pNodeName].x, m_NodePosition[pNodeName].y, m_NodePosition[pNodeName].z);
		return &pos;
	}

	aiVector3D aiVectorLerp(aiVector3D v1, aiVector3D v2, float blend);

private:
	MESH* m_Mesh;
	unsigned int m_MeshNum;
	//const aiScene* m_Scene;
	const aiScene* m_Scene[3];

	ID3D11Buffer* m_VertexBuffer = nullptr;
	ID3D11Buffer* m_IndexBuffer = nullptr;
	unsigned int  m_IndexNum;
	std::unordered_map<std::string, ANIMATION> m_Animation;
	std::unordered_map < std::string, aiQuaternion > m_NodeRotation;
	std::unordered_map < std::string, aiVector3D >	 m_NodePosition;

	int m_CurrentAnimIndex = 0;
	int m_PastAnimIndex = 0;
	float m_Blend = 0.0f;

	// デバッグ用
	int m_Frame = 0;
	bool m_Stop = false;

};

#endif // !MODEL_ANIMATION_H_
