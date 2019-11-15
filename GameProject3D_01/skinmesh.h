#ifndef SKINMESH_H_
#define SKINMESH_H_

#define NUM_BONES_PER_VEREX 4

struct VertexBoneData
{
	unsigned int IDs[NUM_BONES_PER_VEREX];
	float Weights[NUM_BONES_PER_VEREX];
};

class CSkinMesh
{
private :
	const aiScene* m_pScene = nullptr;	// パーシング後のデータ格納用

	bool LoadMesh(const string& pFilename);
	bool InitFromScene(const aiScene* pScene, const string& Filename);

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

};

#endif // !SKINMESH_H_

