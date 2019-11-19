#ifndef PLAYER_H_
#define PLAYER_H_

//class CModel;
class CModelAnimation;
class CCamera;
class CSkinModel;

class CPlayer : public CGameObject
{
private:
	const float m_DefaultSpeed = 0.01f;

	CCamera*          m_pCamera;
	//CModel*         m_pModel;
	CSkinModel*       m_pModel;
	Vector3X3         m_DirVec;
	CCollisionSphere* m_CollisionSphere;
	CCollisionOBB*    m_CollisionOBB;
	CDamage*          m_DamageManager;
	CTexture*         m_Texture;
	Vector3			  m_MoveDistance;
	float             m_MoveSpeed;
	bool			  m_Collision;

	bool IsLanding();
	void Move();
	void Action();

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

