#ifndef STATE_PLAYER_MOVE_WALK_H_
#define STATE_PLAYER_MOVE_WALK_H_

#include "state_enemy_move.h"

class CStateEnemyMoveWalk : public CStateEnemyMove
{
public:
	CStateEnemyMoveWalk(CEnemy* pEnemy, float StartSpeed);
	virtual ~CStateEnemyMoveWalk();
	virtual void Update(CEnemy* pEnemy) override{}
	virtual void UpdateMoveState(CStateEnemyMove* pMoveState, CEnemy* pEnemy) override;

private:
	CStateEnemyMoveWalk(){} // デフォルトコンストラクタ封印
	void Move(CEnemy* pEnemy);

private:
	float m_MoveSpeed;
	float m_StartSpeed;
	float m_TargetSpeed;
	float m_StartLength;
	float m_TargetLength;
	int   m_FrameCounter;
	Vector3 m_Volocity;

};


#endif // !STATE_PLAYER_MOVE_RUN_H_

