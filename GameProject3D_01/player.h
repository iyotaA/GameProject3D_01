#ifndef PLAYER_H_
#define PLAYER_H_

//class CModel;
class CModelAnimation;
class CCamera;
class CSkinModel;

class CPlayer : public CGameObject
{
private:
	CCamera* m_pCamera;
	//CModel* m_pModel;
	CSkinModel* m_pModel;
	Dir3Vector m_DirVec;
	CCollisionSphere* m_CollisionSphere;
	CCollisionOBB* m_CollisionOBB;
	CDamage* m_DamageManager;
	CTexture* m_Texture;

	bool IsLanding();

public:

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void DrawGUI();

	void SetCamera(CCamera* pCamera);
	CCollisionSphere* GetCollisionSphere(void) { return m_CollisionSphere; }
	CDamage* GetDamageManager(void) { return m_DamageManager; }
};

#endif // !PLAYER_H_

