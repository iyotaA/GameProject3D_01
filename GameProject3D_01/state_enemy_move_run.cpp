#include "game_objects_all.h"
#include "state_enemy_move_run.h"
#include "modelAnimation.h"
#include "enemy.h"
#include "MathFunc.h"


CStateEnemyMoveRun::CStateEnemyMoveRun(CEnemy* pEnemy, float StartSpeed)
	: m_MoveSpeed(1.0f)
	, m_StartSpeed(StartSpeed)
	, m_TargetSpeed(1.25f)
	, m_StartLength(CCameraManager::GetCamera()->GetLengthToAt())
	, m_TargetLength(5.5f)
	, m_FrameCounter(0)
	, m_Volocity(Vector3(0.0f, 0.0f, 0.0f))
{
	pEnemy->SetAnimation(ENEMY_STATE_RUN, 1.0f);
	pEnemy->SetAnimationSpeed(1.0f);
}

CStateEnemyMoveRun::~CStateEnemyMoveRun()
{

}

void CStateEnemyMoveRun::UpdateMoveState(CStateEnemyMove* pMoveState, CEnemy* pEnemy)
{
	// カウンター更新
	m_FrameCounter++;
}

void CStateEnemyMoveRun::Move(CEnemy* pEnemy)
{
}
