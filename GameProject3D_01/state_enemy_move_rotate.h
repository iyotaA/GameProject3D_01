#ifndef STATE_ENEMY_MOVE_ROTATE_H_
#define STATE_ENEMY_MOVE_ROTATE_H_

#include "state_enemy_move.h"
class CStateEnemyMoveRotate : public CStateEnemyMove
{
public:
	CStateEnemyMoveRotate(CEnemy* pEnemy);
	virtual ~CStateEnemyMoveRotate();
	virtual void Update(CEnemy* pEnemy) override{}
	virtual void UpdateMoveState(CStateEnemyMove* pMoveState, CEnemy* pEnemy) override;

private:
	CStateEnemyMoveRotate(){} // デフォルトコンストラクタ封印
	bool Rotate(CEnemy* pEnemy);

private:
	float m_StartRadian;
	float m_TargetRadian;
	int   m_FrameCounter;

};


#endif // !STATE_ENEMY_MOVE_ROTATE_H_

