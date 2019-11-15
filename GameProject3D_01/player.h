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
	CDamage* m_DamageManager;
	CTexture* m_Texture;

public:

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void DrawGUI();

	const XMFLOAT3 GetPosition() { return m_Position; }
	void SetCamera(CCamera* pCamera);
	CCollisionSphere* GetCollisionSphere(void) { return m_CollisionSphere; }
	CDamage* GetDamageManager(void) { return m_DamageManager; }
};

#endif // !PLAYER_H_

