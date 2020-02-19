#ifndef PLAYER_H_
#define PLAYER_H_

#include "state_player.h"

class CCamera;
class CSkinModel;
class CNumber;
class CImage;
class CItem;

class CPlayer : public CGameObject
{
public:

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void DrawGUI();
	const Vector3 GetAt() { return m_Position; }

	std::vector< CCollisionWithBone*> GetCollisionSphere(void) const { return m_CollisionBody; }
	CCollisionWithBone* GetCollisionSphere(int _id) const { return m_CollisionBody[_id]; }
	CSkinModel* GetModel(void) { return m_pModel; }
	Vector3 GetFront() { return m_DirVec.front; }

	void ChangeState(CStatePlayer* _pState);
	void AddVelocity(Vector3 _velocity) { m_Position += _velocity; }
	void UseItem();

	void SetMoveSpeed(const float _speed) { m_MoveSpeed = _speed * m_DEFAULT_SPEED; }
	void SetAnimation(const unsigned int _id, const float _endBlendNum);
	void SetAnimationSpeed(const float _speed);
	void SetMotionStop(const bool _stop);
	void ReSpwan();	// リスポーン

public:
	bool& AnimationBlending();
	bool CurrentAnimationFinish();
	int GetCurrentAnimFrameNum();

	SWORD_STATE& WeaponState() { return m_WeaponState; }
	const float DefaultSpeed() { return m_DEFAULT_SPEED; }

	int& DeadTimes() { return m_Status.DeadTimes; }
	int& Life() { return m_Status.Life; }
	float& Stamina() { return m_Status.Stamina; }
	float& AttackValue() { return m_Status.AttackValue; }

	bool& Idle() { return m_StateFlags.Idle; }
	bool& Damaged() { return m_StateFlags.Damage; }
	bool& Dodged() { return m_StateFlags.Dodge; }
	bool& Attacked() { return m_StateFlags.Attack; }
	bool& Blocked() { return m_StateFlags.Block; }
	bool& Dashed() { return m_StateFlags.Dash; }
	bool& Moved() { return m_StateFlags.Move; }
	bool& Died() { return m_StateFlags.Died; }
	bool& EmptyStamina() { return m_StateFlags.StaminaEmpty; }


private:
	bool IsLanding();
	void Move();
	void AddGlavity();
	void UpdateCollision();
	void DrawCollisionGrid();
	void CheckCollision_Enemy();

private:
	const float			m_DEFAULT_SPEED = 0.017f;
	const Vector3		m_SPAWN_POSITION = Vector3(0.0f, 0.0f, -80.0f);

	std::vector< CCollisionWithBone*> m_CollisionBody;
	std::vector< CCollisionSphere*> m_CollisionWeapon;

	SWORD_STATE					m_WeaponState;
	std::vector<std::string>		m_WeaponBoneName;
	std::vector<CImage*>		m_UIAccesser;

	CSkinModel*			m_pModel;
	CSkinModel*			m_pWeapon;
	CStatePlayer*		m_pState;
	Vector3X3				m_DirVec;
	Vector3					m_MoveDistance;
	CPolygon3D*			m_Shadow;
	CNumber*				m_Number;
	float						m_MoveSpeed;
	bool						m_Collision;
	int							m_FrameCounter;
	CItem*					m_Item;

	struct Status
	{
		int DeadTimes;	//	死んだ回数
		int Life;				// ライフ
		float Stamina;	// スタミナ
		float AttackValue;// 攻撃力
	};
	Status	m_Status;

	struct StateFlag
	{
		bool Idle;
		bool Damage;
		bool Dodge;
		bool Attack;
		bool Block;
		bool Dash;
		bool Move;
		bool Died;
		bool StaminaEmpty;
	};
	StateFlag	m_StateFlags;
};

#endif // !PLAYER_H_

