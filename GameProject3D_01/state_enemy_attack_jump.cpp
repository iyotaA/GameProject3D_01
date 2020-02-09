#include "game_objects_all.h"
#include "state_enemy_attack_jump.h"
#include "modelAnimation.h"
#include "enemy.h"

CStateEnemyAttackJump::CStateEnemyAttackJump(CEnemy* pEnemy, bool isMove)
	: m_FrameCounter(0)
	, m_Velocity(Vector3())
	, m_DirFront(Vector3(0.0f, 0.0f, 1.0f))
	, m_Move(isMove)
{
	pEnemy->SetAnimation(ENEMY_STATE_ATTACK_JUMP, 1.0f);
	pEnemy->SetAnimationSpeed(1.5f);
}

CStateEnemyAttackJump::~CStateEnemyAttackJump()
{

}

void CStateEnemyAttackJump::UpdateAttackState(CStateEnemyAttack* pAttackState, CEnemy* pEnemy)
{

}

void CStateEnemyAttackJump::Move(CEnemy* pEnemy)
{

}
