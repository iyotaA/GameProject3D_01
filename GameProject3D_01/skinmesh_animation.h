#ifndef SKINMESH_ANIMATION_H_
#define SKINMESH_ANIMATION_H_

// ライブラリ
#pragma comment(lib, "assimp.lib")

// インクルード
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/matrix4x4.h>

struct MESH
{
	ID3D11Buffer* VertexBuffer;
	ID3D11Buffer* IndexBuffer;
	unsigned int  IndexNum;
	MATERIAL* Material;
};

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
public:
	void Load(char* pFileName, float size, char* pTexture, char* output_fileName);
	void Unload();
	void Draw(XMMATRIX* world);
	void update(int addAnimationFrame);
	Vector3 GetWorldPosition( XMMATRIX* _world,const char* _bone_name);
	XMMATRIX* GetBoneMatrix(XMMATRIX* _world, const char* _bone_name);

	void StopMotion(bool isStop) { m_StopMotion = isStop; }
	void DrawNormal(bool DrawNormal) { m_DrawNormals = DrawNormal; }

	// 引数 / _next : true = Next / _next : false = Back
	void SetAnimation(bool _next);
	void SetAnimation(const unsigned int _id, const float _end_blend_num);
	bool CurrentAnimationFinish();
	int  GetCurrentAnimFrameNum();

	char GetCurrentAnimId() { return m_CurrentAnimId; }
	void SwitchFlag() { m_StopMotion = m_StopMotion ? false : true; }
	void SetAnimationSpeed(float _speed) { m_AnimationSpeed = _speed; }

	int GetAnimationNum() { return m_pScene->mNumAnimations; }
	float* AnimationSpeed() { return &m_AnimationSpeed; }
	bool*  IsStopMotion() { return &m_StopMotion; }
	bool*  DrawAtLine() { return &m_DrawAtLine; }
	int*   MotionFrame() { return &m_MotionFrame; }
	char*  GetCurrentAnimName(){ return m_pScene->mAnimations[m_CurrentAnimId]->mName.data; }
	bool&  AnimationBlending() { return m_AnimationBlending; }

private:
	std::unordered_map<std::string, CTexture*>	m_Texture;
	std::unordered_map<std::string, BONE>		m_Bone;
	std::vector<DEFORM_VERTEX>* m_pDeformVertex = NULL;

	const aiScene* m_pScene = NULL;	// パーシング後のデータ格納用
	MESH* m_Mesh = nullptr;
	FileType m_FileType;
	CShaderDefault* m_Shader;
	float    m_CurrentAnimationFrame = 0.0f;
	float    m_TargetAnimationFrame = 0.0f;
	float    m_Size;

	int  m_MotionFrame = 0;
	bool m_StopMotion = false;
	bool m_DrawAtLine = false;
	bool m_DrawNormals = false;
	bool m_AnimationBlending = false;
	char m_CurrentAnimId = 0;
	char m_TargetAnimId = 0;
	float m_AnimationSpeed = 1.0f;
	float m_PerBlend = 0.0f;
	float m_PerBlendEnd = 0.0f;

	// アニメーション・描画 ////////////////////////////////////
	void DrawStaticMesh();
	void DrawMesh(const aiNode* pNode);
	void LoadMesh(const aiNode* pNode);
	void LoadStaticMesh();
	void CreateBone(aiNode* pNode);
	void CalculateBoneMatrix();
	void UpdateBoneMatrix(aiNode* pNode, aiMatrix4x4 matrix);
	void AnimationBlend(int addAnimationFrame);
	void Animation(int addAnimationFrame);
	void GetBonePosition( aiNode* pNode, XMMATRIX* _world, const char* _targetName, XMMATRIX& _target );
	aiNode* GetBoneNode(aiNode* pNode, const char* _name);

	//変換系 ////////////////////////////////////
	void aiVector3D_Lerp(aiVector3D& _blendVec, const aiVector3D _vec1, const aiVector3D _vec2, float _blend);
	XMFLOAT4X4 LoadAiMatrix4x4(aiMatrix4x4* _martrix_ai);
	void StoreAiMatrix4x4(XMFLOAT4X4* _matrix_ai, aiMatrix4x4& _martrix_ai);

	XMMATRIX PickupOffset(const XMMATRIX& mWorld);
	XMMATRIX PickupScaling(const XMMATRIX& mWorld);
	XMMATRIX PickupRotation(const XMMATRIX& mWorld);

	//ファイル操作・チェック ////////////////////////////////////
	FileType ChackFileType(std::string pFileType);
	void WritteName(aiNode* pNode, ofstream* fileName);

};


#endif // !SKINMESH_ANIMATION_H_

