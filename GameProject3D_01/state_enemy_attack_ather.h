#ifndef STATE_ENEMY_ATTACK_ATHER_H_
#define STATE_ENEMY_ATTACK_ATHER_H_

#include "state_enemy_attack.h"
class CStateEnemyAttackAther : public CStateEnemyAttack
{
public:
	CStateEnemyAttackAther(CEnemy* pEnemy);
	virtual ~CStateEnemyAttackAther();
	virtual void Update(CEnemy* pEnemy) override{}
	virtual void UpdateAttackState(CStateEnemyAttack* pAttackState, CEnemy* pEnemy) override;

private:
	CStateEnemyAttackAther() {} // デフォルトコンストラクタ封印
	void Move(CEnemy* pEnemy);

private:
	int m_FrameCounter;
	Vector3 m_Velocity;
	Vector3 m_DirFront;

};

#endif // !STATE_ENEMY_ATTACK_ATHER_H_
