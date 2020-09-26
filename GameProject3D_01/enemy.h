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
	const Vector3 GetAt() { return m_Position; }

	// ダメージを受ける
	void DealDamage(int _damage) {
		if (m_Status.Life > _damage) {
			m_Status.Life -=  _damage;
		}else {
			m_Status.Life = 0;
			m_StateFlags.Died = true;
		}
	}

	// セッタ
	void ChangeState(CStateEnemy* pState);
	void AddVelocity(Vector3 velocity) { m_Position += velocity; }
	void SetMoveSpeed(const float _speed) { m_MoveSpeed = _speed * m_DEFAULT_SPEED; }
	void SetAnimation(const unsigned int _id, const float _endBlendNum);
	void SetAnimationSpeed(const float _speed);
	void SetMotionStop(const bool _stop);

	// ゲッタ
	std::vector<CCollisionWithBone*> GetCollisionSphere(void) const { return m_CollisionBody; }
	CCollisionWithBone* GetCollisionSphere(int id) { return m_CollisionBody[id]; }
	Vector3 GetFront() const{ return m_DirVec.front; }
	Vector3 GetCenterPosition() const{ return m_CollisionBody[1]->GetSphere()->GetCenter(); }
	CSkinModel* GetModel() { return m_pModel; }
	bool CurrentAnimationFinish()const;
	int GetCurrentAnimFrameNum()const;

	// メンバ参照
	bool& AnimationBlending();
	bool& Damaged() { return m_StateFlags.Damage; }
	bool& Died() { return m_StateFlags.Died; }
	bool& Scouting() { return m_StateFlags.Scouting; }
	bool& Attacked() { return m_StateFlags.Attack; }
	bool& Battle() { return m_StateFlags.Battle; }
	bool& InNearArea() { return m_StateFlags.InNearArea; }
	bool& InNearFrontArea() { return m_StateFlags.InNearFrontArea; }
	bool& InScoutingArea() { return m_StateFlags.InScoutingArea; }
	bool& CollisionTerrian() { return m_StateFlags.CollisionTerrian; }
	int& Life() { return m_Status.Life; }
	int& Attack() { return m_Status.Attack; }
	Vector3& Rotation() { return m_Rotation; }


private:
	bool IsLanding();
	void Move();
	void Action();
	void AddGlavity();
	void UpdateCollision();
	void DrawCollisionGrid();
	void CheckCollision();

private:
	std::vector<CCollisionWithBone*>	m_CollisionBody;
	std::vector <CCollisionSphere*>		m_JudgeArea;

	CSkinModel*		m_pModel;
	Vector3X3			m_DirVec;
	Vector3				m_MoveDistance;
	CPolygon3D*     m_Shadow;
	CStateEnemy*   m_pState;
	float					m_MoveSpeed;
	bool					m_IsCollision;
	bool					m_IsPressMovingEntry;

	const float m_DEFAULT_SPEED = 0.017f;

	struct Status
	{
		int Life;
		int Attack;
	};
	Status	m_Status;

	struct StateFlag
	{
		bool Damage;	// ダメージでひるむ
		bool Attack;		// 攻撃有効中
		bool Scouting;	// 索敵状態
		bool Battle;		// 戦闘状態
		bool Died;			// 死亡
		bool InScoutingArea;	// 索敵範囲内
		bool InNearArea;		// 近距離攻撃範囲内
		bool InNearFrontArea;// 近距離前方攻撃範囲内
		bool CollisionTerrian;	// 地形と当たっているか
	};
	StateFlag	m_StateFlags;

};


#endif // !ENEMY_H_

