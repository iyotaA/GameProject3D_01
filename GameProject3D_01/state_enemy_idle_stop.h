#ifndef STATE_ENEMY_IDLE_STOP_H_
#define STATE_ENEMY_IDLE_STOP_H_

#include "state_enemy_idle.h"
class CStateEnemyIdleStop : public CStateEnemyIdle
{
public:
	CStateEnemyIdleStop(CEnemy* pEnemy);
	~CStateEnemyIdleStop();
	void Update(CEnemy* pEnemy) override{}
	virtual void UpdateIdleState(CStateEnemyIdle* pIdleState, CEnemy* pEnemy) override;

private:
	CStateEnemyIdleStop() {}	// デフォルトコンストラクタ封印
	void Move(CEnemy* pEnemy);
	void Action(CEnemy* pEnemy);

private:
	int m_FrameCounter;
	ENEMY_STATE m_TargetID;

};

#endif // !STATE_PLAYER_IDLE_H_
