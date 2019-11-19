#ifndef SKINMESH_ANIMATION_H_
#define SKINMESH_ANIMATION_H_

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
	// std::string�@Name; �{�[���̖��O���擾�ł����
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
	std::unordered_map<std::string, CTexture*>	m_Texture;
	std::unordered_map<std::string, BONE>		m_Bone;
	std::vector<DEFORM_VERTEX>*					m_pDeformVertex = nullptr;
	const aiScene*								m_pScene = nullptr;	// �p�[�V���O��̃f�[�^�i�[�p
	MESH*    m_Mesh = nullptr;
	FileType m_FileType;
	float    m_AnimationFrame = 0.0f;
	float    m_Size;

	int  m_MotionFrame = 0;
	bool m_IsStopMotion = false;
	bool m_DrawAtLine = false;
	bool m_IsDrawNormals = false;
	char m_CurrentAnimId = 0;
	float m_AnimationSpeed = 1.0f;

	void DrawMesh(const aiNode* pNode);
	void LoadMesh(const aiNode* pNode);
	void CreateBone(aiNode* pNode);
	void UpdateBoneMatrix(aiNode* pNode, aiMatrix4x4 matrix);


public:
	void Load(char* pFileName, float size);
	void Unload();
	void Draw(XMMATRIX* world);
	void Animation(int addAnimationFrame);

	void StopMotion(bool isStop) { m_IsStopMotion = isStop; }
	void DrawNormal(bool DrawNormal) { m_IsDrawNormals = DrawNormal; }

	// ���� / _next : true = Next / _next : false = Back
	void SetAnimation(bool _next) {

		m_CurrentAnimId += _next ? 1: -1;

		if (m_CurrentAnimId == m_pScene->mNumAnimations) {
			m_CurrentAnimId = 0;
		}
		else if (m_CurrentAnimId < 0) {
			m_CurrentAnimId = m_pScene->mNumAnimations - 1;
		}
	}

	void SetAnimation(int _id) { m_CurrentAnimId = _id; }
	char GetCurrentAnimId() { return m_CurrentAnimId; }
	void SwitchFlag() { m_IsStopMotion = m_IsStopMotion ? false : true; }
	void SetAnimationSpeed(float _speed) { m_AnimationSpeed = _speed; }

	float* AnimationSpeed() { return &m_AnimationSpeed; }
	int* MotionFrame() { return &m_MotionFrame; }
	char* GetCurrentAnimName(){ return m_pScene->mAnimations[m_CurrentAnimId]->mName.data; }
	bool* IsStopMotion() { return &m_IsStopMotion; }
	bool* DrawAtLine() { return &m_DrawAtLine; }
};


#endif // !SKINMESH_ANIMATION_H_

