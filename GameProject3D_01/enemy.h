#ifndef ENEMY_H_
#define ENEMY_H_

class CSkinModel;
class CStateEnemy;

class CEnemy : public CGameObject
{
public:

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void DrawGUI();

	void ChangeState(CStateEnemy* pState);
	std::vector<CCollisionWithBone*> GetCollisionSphere(void) const { return m_CollisionBody; }

	void AddVelocity(Vector3 velocity) { m_Position += velocity; }
	void SetMoveSpeed(const float _speed) { m_MoveSpeed = _speed * m_DEFAULT_SPEED; }
	void SetAnimation(const unsigned int _id, const float _endBlendNum);
	void SetAnimationSpeed(const float _speed);
	CSkinModel* GetModel() { return m_pModel; }

	bool& AnimationBlending();
	bool CurrentAnimationFinish()const;
	int GetCurrentAnimFrameNum()const;
	Vector3 GetFront() const{ return m_DirVec.front; }

	bool& Damaged() { return m_StateFlags.Damage; }
	bool& Dodged() { return m_StateFlags.Dodge; }
	bool& Attacked() { return m_StateFlags.Attack; }
	bool& Blocked() { return m_StateFlags.Block; }
	Vector3& Rotation() { return m_Rotation; }


private:
	bool IsLanding();
	void Move();
	void Action();
	void AddGlavity();
	void UpdateCollision();
	void DrawCollisionGrid();

private:
	std::vector<CCollisionWithBone*> m_CollisionBody;

	CSkinModel*       m_pModel;
	Vector3X3         m_DirVec;
	CCollisionOBB*    m_CollisionOBB;
	Vector3			  m_MoveDistance;
	CPolygon3D*       m_Shadow;
	CStateEnemy*      m_pState;
	float             m_MoveSpeed;
	bool			  m_IsCollision;
	bool			  m_IsPressMovingEntry;

	const float m_DEFAULT_SPEED = 0.017f;

	struct StateFlag
	{
		bool Damage;
		bool Dodge;
		bool Attack;
		bool Block;
	};
	StateFlag	m_StateFlags;

};


#endif // !ENEMY_H_

