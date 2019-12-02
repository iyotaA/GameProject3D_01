#ifndef PLAYER_H_
#define PLAYER_H_

class CCamera;
class CSkinModel;

class CPlayer : public CGameObject
{
public:

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void DrawGUI();

	CCollisionSphere* GetCollisionSphere(void) { return m_CollisionSphere; }
	CDamage* GetDamageManager(void) { return m_DamageManager; }

private:
	const float m_DefaultSpeed = 0.017f;

	CSkinModel*       m_pModel;
	Vector3X3         m_DirVec;
	CCollisionSphere* m_CollisionSphere;
	CCollisionOBB*    m_CollisionOBB;
	CDamage*          m_DamageManager;
	Vector3			  m_MoveDistance;
	Vector3			  m_BonePosition;
	float             m_MoveSpeed;
	bool			  m_IsCollision;
	bool			  m_IsPressMovingEntry;

	bool IsLanding();
	void Move();
	void Action();
	void AddGlavity();
	void UpdateCollision();
	void DrawCollisionGrid();

};

#endif // !PLAYER_H_

