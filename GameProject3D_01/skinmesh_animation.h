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
	void Load(char* pFileName, float size);
	void Unload();
	void Draw(XMMATRIX* world);
	void update(int addAnimationFrame);
	Vector3 GetWorldPosition( XMMATRIX* _world,const char* _bone_name);

	void StopMotion(bool isStop) { m_IsStopMotion = isStop; }
	void DrawNormal(bool DrawNormal) { m_IsDrawNormals = DrawNormal; }

	// 引数 / _next : true = Next / _next : false = Back
	void SetAnimation(bool _next);
	void SetAnimation(const unsigned int _id, const float _startBlendNum);

	char GetCurrentAnimId() { return m_CurrentAnimId; }
	void SwitchFlag() { m_IsStopMotion = m_IsStopMotion ? false : true; }
	void SetAnimationSpeed(float _speed) { m_AnimationSpeed = _speed; }

	float* AnimationSpeed() { return &m_AnimationSpeed; }
	bool*  IsStopMotion() { return &m_IsStopMotion; }
	bool*  DrawAtLine() { return &m_DrawAtLine; }
	int*   MotionFrame() { return &m_MotionFrame; }
	char*  GetCurrentAnimName(){ return m_pScene->mAnimations[m_CurrentAnimId]->mName.data; }

private:
	std::unordered_map<std::string, CTexture*>	m_Texture;
	std::unordered_map<std::string, BONE>		m_Bone;
	std::vector<DEFORM_VERTEX>* m_pDeformVertex = NULL;

	const aiScene* m_pScene = NULL;	// パーシング後のデータ格納用
	MESH* m_Mesh = nullptr;
	FileType m_FileType;
	CShaderDefault* m_Shader;
	float    m_AnimationFrame = 0.0f;
	float    m_Size;

	int  m_MotionFrame = 0;
	bool m_IsStopMotion = false;
	bool m_DrawAtLine = false;
	bool m_IsDrawNormals = false;
	bool m_IsAnimationBlending = false;
	char m_CurrentAnimId = 0;
	char m_TargetAnimId = 0;
	float m_AnimationSpeed = 1.0f;
	float m_PerBlend = 0.0f;

	// アニメーション・描画 ////////////////////////////////////
	void DrawMesh(const aiNode* pNode);
	void LoadMesh(const aiNode* pNode);
	void CreateBone(aiNode* pNode);
	void CalculateBoneMatrix();
	void UpdateBoneMatrix(aiNode* pNode, aiMatrix4x4 matrix);
	void AnimationBlend();
	void Animation();
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
	void WritteName(aiNode* pNode);

};


#endif // !SKINMESH_ANIMATION_H_

