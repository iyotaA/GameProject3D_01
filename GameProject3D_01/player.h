#ifndef PLAYER_H_
#define PLAYER_H_

#include "state_player.h"

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

	void ChangeState(CStatePlayer* pState);

	CCollisionSphere* GetCollisionSphere(void) { return m_CollisionSphere; }
	CDamage* GetDamageManager(void) { return m_DamageManager; }
	CSkinModel* GetModel(void) { return m_pModel; }
	void SetMoveSpeed(const float _speed) { m_MoveSpeed = _speed * m_DefaultSpeed; }
	void SetAnimation(const unsigned int _id, const float _endBlendNum);
	void SetAnimationSpeed(const float _speed);

public:
	bool& PressMovingEntry(void) { return m_IsPressMovingEntry; }
	bool& AnimationBlending(void);
	bool CurrentAnimationFinish(void);

private:
	const float m_DefaultSpeed = 0.017f;

	CSkinModel*       m_pModel;
	CStatePlayer*	  m_pState;
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

