#ifndef STATE_ENEMY_MOVE_RUN_H_
#define STATE_ENEMY_MOVE_RUN_H_

#include "state_enemy_move.h"
class CStateEnemyMoveRun : public CStateEnemyMove
{
public:
	CStateEnemyMoveRun(CEnemy* pEnemy, float StartSpeed);
	virtual ~CStateEnemyMoveRun();
	virtual void Update(CEnemy* pEnemy) override{}
	virtual void UpdateMoveState(CStateEnemyMove* pMoveState, CEnemy* pEnemy) override;

private:
	CStateEnemyMoveRun() {} // デフォルトコンストラクタ封印
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

#endif // !STATE_ENEMY_MOVE_RUN_H_
