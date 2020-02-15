#ifndef STATE_ENEMY_IDLE_WATCH_AROUND_H_
#define STATE_ENEMY_IDLE_WATCH_AROUND_H_

#include "state_enemy_idle.h"

class CStateEnemyIdleWatchAround : public CStateEnemyIdle
{
public:
	CStateEnemyIdleWatchAround(CEnemy* pEnemy);
	~CStateEnemyIdleWatchAround();
	void Update(CEnemy* pEnemy) override{}
	virtual void UpdateIdleState(CStateEnemyIdle* pIdleState, CEnemy* pEnemy) override;

private:
	CStateEnemyIdleWatchAround() {}	// デフォルトコンストラクタ封印
	void Move(CEnemy* pEnemy);
	void Action(CEnemy* pEnemy);

private:
	int m_FrameCounter;

};

#endif // !STATE_ENEMY_IDLE_WATCH_AROUND_H_
