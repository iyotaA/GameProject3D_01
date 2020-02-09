#include "game_objects_all.h"
#include "state_enemy_move.h"
#include "state_enemy_idle.h"
#include "state_enemy_attack.h"
#include "state_enemy_attack_jump.h"
#include "state_enemy_damage.h"
#include "modelAnimation.h"
#include "enemy.h"

CStateEnemyAttack::CStateEnemyAttack(CEnemy* pEnemy, ENEMY_STATE state_index)
	: m_Lerp_t(0.0f)
	, m_LerpStart(CCameraManager::GetCamera()->GetLengthToAt())
	, m_LerpEnd(8.0f)
	, m_Counter(0.0f)
{
	if (state_index == ENEMY_STATE_ATTACK_JUMP) {
		m_pStateAttack = new CStateEnemyAttackJump(pEnemy, true);
	}
}

CStateEnemyAttack::~CStateEnemyAttack()
{
	delete m_pStateAttack;
}

void CStateEnemyAttack::Update(CEnemy* pEnemy)
{
	// XV
	m_pStateAttack->UpdateAttackState(this, pEnemy);
}

void CStateEnemyAttack::ChangeState(CStateEnemyAttack* pAttackState)
{
	delete m_pStateAttack;
	m_pStateAttack = pAttackState;
}
