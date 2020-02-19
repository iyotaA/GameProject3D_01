#include "game_objects_all.h"
#include "scene.h"
#include "state_enemy_idle_stop.h"
#include "state_enemy_idle_watch_around.h"
#include "state_enemy_move.h"
#include "state_enemy_attack.h"
#include "modelAnimation.h"
#include "skinmesh_animation.h"
#include "enemy.h"
#include "MathFunc.h"

CStateEnemyIdleStop::CStateEnemyIdleStop(CEnemy* pEnemy)
	: m_FrameCounter(0)
	, m_TargetID(ENEMY_STATE_IDLE_STOP)
{
	pEnemy->SetAnimation(m_TargetID, 1.0f);
	pEnemy->SetAnimationSpeed(0.7f);
}

CStateEnemyIdleStop::~CStateEnemyIdleStop()
{

}

void CStateEnemyIdleStop::UpdateIdleState(CStateEnemyIdle* pIdleState, CEnemy* pEnemy)
{
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	//		2秒間は待機...
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	if (m_FrameCounter++ * DELTA_TIME <= 2.0f)return;


	//--------------------------------------------------------------------------
	// 戦闘状態か？
	if (pEnemy->Battle()) {
		ChangeBattleState(pEnemy);
	}
	else {
		ChangeScoutingState(pIdleState, pEnemy);
	}
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
}


void CStateEnemyIdleStop::ChangeScoutingState(CStateEnemyIdle* pIdleState, CEnemy* pEnemy)
{
	//--------------------------------------------------------------------------
	// 索敵可能範囲内にプレイヤーがいるか？
	if (pEnemy->InScoutingArea()) {
		pEnemy->ChangeState(new CStateEnemyMove(pEnemy, STATE_ROTATE));
		return;
	}

	/// 廻りを見渡す状態に遷移
	pIdleState->ChangeState(new CStateEnemyIdleWatchAround(pEnemy));
}


void CStateEnemyIdleStop::ChangeBattleState(CEnemy* pEnemy)
{
	CPlayer* player = CManager::GetScene()->GetGameObject<CPlayer>(CManager::LAYER_OBJECT);

	//--------------------------------------------------------------------------
	// 正面ベクトルとプレイヤーまでの方向ベクトルの角度と距離を求める
	Vector3 enemy_to_player = *player->GetPosition() - *pEnemy->GetPosition();
	float length = enemy_to_player.Length();

	//--------------------------------------------------------------------------
	// 近距離可能範囲内にプレイヤーがいるか？
	if (pEnemy->InNearArea()) {	// 飛翔攻撃

		// プレイヤーが前方にいるか？
		if (pEnemy->InNearFrontArea()) {
			pEnemy->ChangeState(new CStateEnemyAttack(pEnemy, ENEMY_STATE_ATTACK_HOOK));
			return;
		}

		pEnemy->ChangeState(new CStateEnemyAttack(pEnemy, ENEMY_STATE_ATTACK_JUMP));
		return;
	}


	//--------------------------------------------------------------------------
	// 正面ベクトルとプレイヤーまでの方向ベクトルの角度の差分を求める
	float front_to_player_difference = fabs(atan2f(enemy_to_player.x, enemy_to_player.z) - atan2f(pEnemy->GetFront().x, pEnemy->GetFront().z));


	//--------------------------------------------------------------------------
	// 角度差分が45度より小さければ走行ステートに遷移
	if (front_to_player_difference <= 45.0f * DEGREE_TO_RADIAN) {
		pEnemy->ChangeState(new CStateEnemyMove(pEnemy, STATE_RUN));
		return;
	}
	else {// 角度差分が45度より大きければ回転ステートに遷移
		pEnemy->ChangeState(new CStateEnemyMove(pEnemy, STATE_ROTATE));
		return;
	}
}

