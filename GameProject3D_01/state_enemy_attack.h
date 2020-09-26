#ifndef STATE_ENEMY_ATTACK_H_
#define STATE_ENEMY_ATTACK_H_


#include "state_enemy.h"

class CStateEnemyAttack : public CStateEnemy
{
public:
	CStateEnemyAttack(CEnemy* pEnemy, ENEMY_STATE state_index);
	virtual ~CStateEnemyAttack();
	virtual void Update(CEnemy* pEnemy) override;
	virtual void UpdateAttackState(CStateEnemyAttack* pAttackState, CEnemy* pEnemy) {};

	void ChangeState(CStateEnemyAttack* pAttackState);

protected:
	CStateEnemyAttack() {} // デフォルトコンストラクタ封印
	bool MoveEntry() {
		return CInput::GetIsInputStick(LEFT_STICK) ||
			CInput::GetKeyPress('W') || CInput::GetKeyPress('A') ||
			CInput::GetKeyPress('S') || CInput::GetKeyPress('D');
	}

private:
	CStateEnemyAttack* m_pStateAttack;
	float m_Lerp_t;
	float m_LerpStart;
	float m_LerpEnd;
	float m_Counter;

};

class CStateEnemyAttackNone : public CStateEnemyAttack
{
public:
	CStateEnemyAttackNone(CEnemy* pEnemy){}
	virtual ~CStateEnemyAttackNone(){}
	virtual void Update(CEnemy* pEnemy) override{}
	virtual void UpdateAttackState(CStateEnemyAttack* pAttackState, CEnemy* pEnemy) {};
};

#endif // !STATE_ENEMY_ATTACK_H_
