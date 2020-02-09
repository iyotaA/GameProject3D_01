#ifndef STATE_ENEMY_IDLE_H_
#define STATE_ENEMY_IDLE_H_

#include "state_enemy.h"

class CStateEnemyIdle : public CStateEnemy
{
public:
	CStateEnemyIdle(CEnemy* pEnemy);
	~CStateEnemyIdle();
	void Update(CEnemy* pEnemy) override;
	virtual void UpdateIdleState(CStateEnemyIdle* pIdleState, CEnemy* pEnemy) {};

protected:
	CStateEnemyIdle() {}	// デフォルトコンストラクタ封印

private:
	CStateEnemyIdle* m_pStateIdle;
	int m_FrameCounter;

};

#endif // !STATE_PLAYER_IDLE_H_
