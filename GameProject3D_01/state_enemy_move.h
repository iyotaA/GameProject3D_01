#ifndef STATE_ENEMY_MOVE_H_
#define STATE_ENEMY_MOVE_H_

#include "state_enemy.h"

typedef enum
{
	STATE_ROTATE,
	STATE_RUN,
	STATE_WALK,
}ENEMY_STATE_MOVE;

class CStateEnemyMove : public CStateEnemy
{
public:
	CStateEnemyMove(CEnemy* pEnemy, ENEMY_STATE_MOVE state_move);
	virtual ~CStateEnemyMove();
	virtual void Update(CEnemy* pEnemy) override;
	virtual void UpdateMoveState(CStateEnemyMove* pMoveState, CEnemy* pEnemy) {};

	void ChangeState(CStateEnemyMove* pMoveState);

protected:
	CStateEnemyMove(){} // デフォルトコンストラクタ封印

private:
	bool Action(CEnemy* pEnemy);

private:
	CStateEnemyMove* m_pStateMove;
	int m_FrameCounter;

};

class CStateEnemyMoveNone : public CStateEnemyMove
{
public:
	CStateEnemyMoveNone(CEnemy* pEnemy) {}
	virtual ~CStateEnemyMoveNone() {}
	virtual void Update(CEnemy* pEnemy) override {}
	virtual void UpdateAttackState(CStateEnemyMove* pMoveState, CEnemy* pEnemy) {};
};


#endif // !STATE_ENEMY_MOVE_H_
