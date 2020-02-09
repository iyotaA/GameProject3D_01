#ifndef STATE_ENEMY_ATTACK_HOOK_H_
#define STATE_ENEMY_ATTACK_HOOK_H_

#include "state_enemy_attack.h"
class CStateEnemyAttackHook : public CStateEnemyAttack
{
public:
	CStateEnemyAttackHook(CEnemy* pEnemy);
	virtual ~CStateEnemyAttackHook();
	virtual void Update(CEnemy* pEnemy) override{}
	virtual void UpdateAttackState(CStateEnemyAttack* pAttackState, CEnemy* pEnemy) override;

private:
	CStateEnemyAttackHook() {} // デフォルトコンストラクタ封印
	void Move(CEnemy* pEnemy);

private:
	int m_FrameCounter;
	Vector3 m_Velocity;
	Vector3 m_DirFront;

};

#endif // !STATE_ENEMY_ATTACK_HOOK_H_
