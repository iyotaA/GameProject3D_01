#include "game_objects_all.h"
#include "state_enemy_idle_stop.h"
#include "state_enemy_move.h"
#include "modelAnimation.h"
#include "skinmesh_animation.h"
#include "enemy.h"
#include "MathFunc.h"

CStateEnemyIdleStop::CStateEnemyIdleStop(CEnemy* pEnemy)
	: m_FrameCounter(0)
	, m_TargetID(ENEMY_STATE_IDLE_STOP)
{
	pEnemy->SetAnimation(m_TargetID, 1.0f);
	pEnemy->SetAnimationSpeed(1.0f);
}

CStateEnemyIdleStop::~CStateEnemyIdleStop()
{

}

void CStateEnemyIdleStop::UpdateIdleState(CStateEnemyIdle* pIdleState, CEnemy* pEnemy)
{
	// 入力・状態遷移
	Action(pEnemy);

	// 移動処理
	//Move(pEnemy);

	// カウンター更新
	m_FrameCounter++;
}

void CStateEnemyIdleStop::Move(CEnemy* pEnemy)
{
	if (CInput::GetGamepadTrigger(XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
		int temp = (int)m_TargetID;
		temp++;
		m_TargetID = (ENEMY_STATE)temp;
		if (m_TargetID > ENEMY_STATE_WALK) {
			m_TargetID = ENEMY_STATE_ATTACK_HOOK;
		}
		pEnemy->SetAnimation(m_TargetID, 1.0f);
	}
	else if (CInput::GetGamepadTrigger(XINPUT_GAMEPAD_LEFT_SHOULDER)) {
		int temp = (int)m_TargetID;
		temp--;
		m_TargetID = (ENEMY_STATE)temp;
		if (m_TargetID > ENEMY_STATE_ATTACK_HOOK) {
			m_TargetID = ENEMY_STATE_WALK;
		}
		pEnemy->SetAnimation(m_TargetID, 1.0f);
	}
}

void CStateEnemyIdleStop::Action(CEnemy* pEnemy)
{
	if (m_FrameCounter <= 150)return;
	pEnemy->ChangeState(new CStateEnemyMove(pEnemy, STATE_ROTATE));
}

