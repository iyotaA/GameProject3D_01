#ifndef STATE_ENEMY_DAMAGE_H_
#define STATE_ENEMY_DAMAGE_H_

#include "state_enemy.h"
class CStateEnemyDamage : public CStateEnemy
{
public:
	CStateEnemyDamage(CEnemy* pEnemy);
	virtual ~CStateEnemyDamage();
	virtual void Update(CEnemy* pEnemy) override;

protected:
	CStateEnemyDamage() {} // デフォルトコンストラクタ封印

private:

};

class CStateEnemyDamageNone : public CStateEnemyDamage
{
public:
	CStateEnemyDamageNone(CEnemy* pEnemy) {}
	virtual ~CStateEnemyDamageNone() {}
	virtual void Update(CEnemy* pEnemy) override {}
};

#endif // !STATE_ENEMY_DAMAGE_H_
