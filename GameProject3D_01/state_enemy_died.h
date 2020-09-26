#ifndef STATE_ENEMY_DIED_H_
#define STATE_ENEMY_DIED_H_


#include "state_enemy.h"
class CStateEnemyDied : public CStateEnemy
{
public:
	CStateEnemyDied(CEnemy* pEnemy);
	virtual ~CStateEnemyDied();
	virtual void Update(CEnemy* pEnemy) override;

private:
	CStateEnemyDied() {} // デフォルトコンストラクタ封印

	int m_FrameCounter;
};

#endif // !STATE_ENEMY_DIED_H_
