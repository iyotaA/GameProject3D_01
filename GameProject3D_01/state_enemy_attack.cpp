#include "game_objects_all.h"
#include "state_enemy_move.h"
#include "state_enemy_idle.h"
#include "state_enemy_attack.h"
#include "state_enemy_attack_jump.h"
#include "state_enemy_attack_hook.h"
#include "state_enemy_damage.h"
#include "state_enemy_died.h"
#include "enemy.h"

CStateEnemyAttack::CStateEnemyAttack(CEnemy* pEnemy, ENEMY_STATE state_index)
	: m_Lerp_t(0.0f)
	, m_LerpStart(CCameraManager::GetCamera()->GetLengthToAt())
	, m_LerpEnd(8.0f)
	, m_Counter(0.0f)
{
	switch (state_index)
	{
	case ENEMY_STATE_ATTACK_JUMP:
		m_pStateAttack = new CStateEnemyAttackJump(pEnemy, true);
		break;
	case ENEMY_STATE_ATTACK_HOOK:
		m_pStateAttack = new CStateEnemyAttackHook(pEnemy);
		break;
	default:
		m_pStateAttack = new CStateEnemyAttackNone(pEnemy);
		return;
	}
}

CStateEnemyAttack::~CStateEnemyAttack()
{
	delete m_pStateAttack;
}

void CStateEnemyAttack::Update(CEnemy* pEnemy)
{
	// 死亡状態に遷移
	if (pEnemy->Died()) {
		pEnemy->ChangeState(new CStateEnemyDied(pEnemy));
		return;
	}

	// 攻撃ステートが無ければ待機ステートに遷移
	if (typeid(*m_pStateAttack) == typeid(CStateEnemyAttackNone)) {
		pEnemy->ChangeState(new CStateEnemyIdle(pEnemy));
		return;
	}

	// 更新
	m_pStateAttack->UpdateAttackState(this, pEnemy);
}

void CStateEnemyAttack::ChangeState(CStateEnemyAttack* pAttackState)
{
	delete m_pStateAttack;
	m_pStateAttack = pAttackState;
}
