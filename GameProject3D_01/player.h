#ifndef PLAYER_H_
#define PLAYER_H_

#include "state_player.h"
#include "number.h"

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
	void AddVelocity(Vector3 velocity) { m_Position += velocity; }
	Vector3 GetFront() { return m_DirVec.front; }

public:
	bool& AnimationBlending();
	bool CurrentAnimationFinish();
	int GetCurrentAnimFrameNum();
	const float DefaultSpeed() { return m_DefaultSpeed; }

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
	CPolygon3D*		  m_Shadow;
	float             m_MoveSpeed;
	bool			  m_IsCollision;

	bool IsLanding();
	void Move();
	void Action();
	void AddGlavity();
	void UpdateCollision();
	void DrawCollisionGrid();

};

#endif // !PLAYER_H_

