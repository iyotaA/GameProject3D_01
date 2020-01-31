#ifndef ENEMY_H_
#define ENEMY_H_

class CEnemy : public CGameObject
{
public:

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void DrawGUI();

	std::vector< CCollisionSphere*> GetCollisionSphere(void) const { return m_CollisionSphere; }
	CDamage* GetDamageManager(void) const{ return m_DamageManager; }

private:
	const float m_DefaultSpeed = 0.017f;

	CSkinModel* m_pModel;
	Vector3X3         m_DirVec;
	std::vector< CCollisionSphere*> m_CollisionSphere;
	CCollisionOBB* m_CollisionOBB;
	CDamage* m_DamageManager;
	Vector3			  m_MoveDistance;
	Vector3			  m_BonePosition;
	CPolygon3D*       m_Shadow;
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


#endif // !ENEMY_H_

