#include "game_objects_all.h"
#include "state_enemy_move_walk.h"
#include "enemy.h"
#include "MathFunc.h"


CStateEnemyMoveWalk::CStateEnemyMoveWalk(CEnemy* pEnemy)
	: m_MoveSpeed(1.0f)
	, m_TargetSpeed(1.0f)
	, m_StartLength(CCameraManager::GetCamera()->GetLengthToAt())
	, m_TargetLength(5.0f)
	, m_FrameCounter(0)
	, m_Volocity(Vector3())
{
	pEnemy->SetAnimation(ENEMY_STATE_WALK, 1.0f);
	pEnemy->SetAnimationSpeed(1.0f);
}

CStateEnemyMoveWalk::~CStateEnemyMoveWalk()
{

}

void CStateEnemyMoveWalk::UpdateMoveState(CStateEnemyMove* pMoveState, CEnemy* pEnemy)
{
	// カウンター更新
	m_FrameCounter++;
}

void CStateEnemyMoveWalk::Move(CEnemy* pEnemy)
{
}
