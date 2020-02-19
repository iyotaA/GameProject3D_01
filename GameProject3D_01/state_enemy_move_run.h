#ifndef STATE_ENEMY_MOVE_RUN_H_
#define STATE_ENEMY_MOVE_RUN_H_

#include "state_enemy_move.h"
class CPlayer;
class CStateEnemyMoveRun : public CStateEnemyMove
{
public:
	CStateEnemyMoveRun(CEnemy* pEnemy);
	virtual ~CStateEnemyMoveRun();
	virtual void Update(CEnemy* pEnemy) override{}
	virtual void UpdateMoveState(CStateEnemyMove* pMoveState, CEnemy* pEnemy) override;

private:
	CStateEnemyMoveRun() {} // デフォルトコンストラクタ封印
	bool Move(CEnemy* pEnemy);

private:
	Vector3 m_MoveSpeed;
	CPlayer* m_PlayerAccesser;
	int   m_FrameCounter;

};

#endif // !STATE_ENEMY_MOVE_RUN_H_
