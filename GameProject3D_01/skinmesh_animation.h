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
	char m_AnimationIndex = 0;

	void DrawMesh(const aiNode* pNode, aiMatrix4x4* _mtx);
	void LoadMesh(const aiNode* pNode);
	void CreateBone(aiNode* pNode);
	void UpdateBoneMatrix(aiNode* pNode, aiMatrix4x4 matrix);


public:
	void Load(char* pFileName, float size);
	void Unload();
	void Draw(XMMATRIX* world);
	void Animation(int frame);
	void SetMotionFrame(int frame) { m_MotionFrame = frame; }
	void StopMotion(bool isStop) { m_IsStopMotion = isStop; }
	void FrameMode(bool IsFrameMode) { m_IsFrameMode = IsFrameMode; }
	void DrawNormal(bool DrawNormal) { m_IsDrawNormals = DrawNormal; }

	void SetNextAnimation() {
		m_AnimationIndex++;
		if (m_AnimationIndex == m_pScene->mNumAnimations) {
			m_AnimationIndex = 0;
		}
	}
};


#endif // !SKINMESH_ANIMATION_H_

