#ifndef SKINMESH_ANIMATION_H_
#define SKINMESH_ANIMATION_H_

using namespace std;
class MESH;

typedef enum {
	E_NONE = -1,
	E_FBX,
	E_OBJ,
}FileType;

struct DEFORM_VERTEX
{
	aiVector3D Position;
	aiVector3D DeformPosition;
	aiVector3D Normal;
	aiVector3D DeformNormal;
	int BoneNum;
	// int BoneIndex[4];
	std::string BoneName[4];
	float BoneWeight[4];
};

struct BONE
{
	// std::string　Name; ボーンの名前を取得できるよ
	aiMatrix4x4 Matrix;
	aiMatrix4x4 AnimationMatrix;
	aiMatrix4x4 OffsetMatrix;
};

//struct MESH
//{
//	ID3D11Buffer* VertexBuffer;
//	ID3D11Buffer* IndexBuffer;
//	unsigned int  IndexNum;
//	MATERIAL* Material;
//};

class CSkinModel
{
private:
	MESH* m_Mesh = nullptr;
	const aiScene* m_pScene = nullptr;	// パーシング後のデータ格納用
	float			m_Size;
	std::unordered_map<std::string, CTexture*>	m_Texture;
	FileType		m_FileType;
	//std::vector<BONE> m_Bone;	// こっちのほうが良い
	std::unordered_map<std::string, BONE> m_Bone;
	std::vector<DEFORM_VERTEX>* m_pDeformVertex;

	int m_MotionFrame = 0;
	bool m_IsStopMotion = false;
	bool m_IsFrameMode = false;
	bool m_IsDrawNormals = false;
	char m_CurrentAnimId = 0;

	void DrawMesh(const aiNode* pNode);
	void LoadMesh(const aiNode* pNode);
	void CreateBone(aiNode* pNode);
	void UpdateBoneMatrix(aiNode* pNode, aiMatrix4x4 matrix);


public:
	void Load(char* pFileName, float size);
	void Unload();
	void Draw(XMMATRIX* world);
	void Animation(int frame);

	void StopMotion(bool isStop) { m_IsStopMotion = isStop; }
	void FrameMode(bool IsFrameMode) { m_IsFrameMode = IsFrameMode; }
	void DrawNormal(bool DrawNormal) { m_IsDrawNormals = DrawNormal; }

	// 引数 / _next : true = Next / _next : false = Back
	void SetAnimIdNext(bool _next) {

		m_CurrentAnimId += _next ? 1: -1;

		if (m_CurrentAnimId == m_pScene->mNumAnimations) {
			m_CurrentAnimId = 0;
		}
		else if (m_CurrentAnimId < 0) {
			m_CurrentAnimId = m_pScene->mNumAnimations - 1;
		}
	}

	char GetCurrentAnimId() { return m_CurrentAnimId; }
	char* GetCurrentAnimName(){ return m_pScene->mAnimations[m_CurrentAnimId]->mName.data; }
	void SwitchFlag() { m_IsStopMotion = m_IsStopMotion ? false : true; }

	bool* IsStopMotion() { return &m_IsStopMotion; }
	int* MotionFrame() { return &m_MotionFrame; }
};


#endif // !SKINMESH_ANIMATION_H_

