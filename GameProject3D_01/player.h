#ifndef PLAYER_H_
#define PLAYER_H_

#include "state_player.h"

class CCamera;
class CSkinModel;
class CNumber;

class CPlayer : public CGameObject
{
public:

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void DrawGUI();

	std::vector< CCollisionWithBone*> GetCollisionSphere(void) const { return m_CollisionBody; }
	CSkinModel* GetModel(void) { return m_pModel; }
	Vector3 GetFront() { return m_DirVec.front; }

	void ChangeState(CStatePlayer* pState);
	void AddVelocity(Vector3 velocity) { m_Position += velocity; }

	void SetMoveSpeed(const float _speed) { m_MoveSpeed = _speed * m_DEFAULT_SPEED; }
	void SetAnimation(const unsigned int _id, const float _endBlendNum);
	void SetAnimationSpeed(const float _speed);

public:
	bool& AnimationBlending();
	bool CurrentAnimationFinish();
	int GetCurrentAnimFrameNum();

	SWORD_STATE& WeaponState() { return m_WeaponState; }
	const float DefaultSpeed() { return m_DEFAULT_SPEED; }

	bool& Damaged() { return m_StateFlags.Damage; }
	bool& Dodged() { return m_StateFlags.Dodge; }
	bool& Attacked() { return m_StateFlags.Attack; }
	bool& Blocked() { return m_StateFlags.Block; }

private:
	bool IsLanding();
	void Move();
	void AddGlavity();
	void UpdateCollision();
	void DrawCollisionGrid();
	void CheckCollision_Enemy();

private:
	const float			m_DEFAULT_SPEED = 0.017f;
	std::vector< CCollisionWithBone*> m_CollisionBody;
	std::vector< CCollisionSphere*> m_CollisionWeapon;

	SWORD_STATE					m_WeaponState;
	std::vector<std::string>	m_WeaponBoneName;

	CSkinModel*			m_pModel;
	CSkinModel*			m_pWeapon;
	CStatePlayer*		m_pState;
	Vector3X3			m_DirVec;
	CCollisionOBB*		m_CollisionOBB;
	Vector3				m_MoveDistance;
	CPolygon3D*			m_Shadow;
	CNumber*			m_Number;
	float				m_MoveSpeed;
	bool				m_Collision;

	struct StateFlag
	{
		bool Damage;
		bool Dodge;
		bool Attack;
		bool Block;
	};
	StateFlag	m_StateFlags;
};

#endif // !PLAYER_H_

