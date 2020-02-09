#ifndef STATE_ENEMY_ATTACK_JUMP_H_
#define STATE_ENEMY_ATTACK_JUMP_H_

#include "state_enemy_attack.h"
class CStateEnemyAttackJump : public CStateEnemyAttack
{
public:
	CStateEnemyAttackJump(CEnemy* pEnemy, bool isMove);
	virtual ~CStateEnemyAttackJump();
	virtual void Update(CEnemy* pEnemy) override{}
	virtual void UpdateAttackState(CStateEnemyAttack* pAttackState, CEnemy* pEnemy) override;

private:
	CStateEnemyAttackJump() {} // デフォルトコンストラクタ封印
	void Move(CEnemy* pEnemy);

private:
	int m_FrameCounter;
	Vector3 m_Velocity;
	Vector3 m_DirFront;
	bool	m_Move;

};

#endif // !STATE_ENEMY_ATTACK_JUMP_H_
