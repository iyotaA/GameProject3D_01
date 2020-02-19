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
	//		2�b�Ԃ͑ҋ@...
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	if (m_FrameCounter++ * DELTA_TIME <= 2.0f)return;


	//--------------------------------------------------------------------------
	// �퓬��Ԃ��H
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
	// ���G�\�͈͓��Ƀv���C���[�����邩�H
	if (pEnemy->InScoutingArea()) {
		pEnemy->ChangeState(new CStateEnemyMove(pEnemy, STATE_ROTATE));
		return;
	}

	/// �������n����ԂɑJ��
	pIdleState->ChangeState(new CStateEnemyIdleWatchAround(pEnemy));
}


void CStateEnemyIdleStop::ChangeBattleState(CEnemy* pEnemy)
{
	CPlayer* player = CManager::GetScene()->GetGameObject<CPlayer>(CManager::LAYER_OBJECT);

	//--------------------------------------------------------------------------
	// ���ʃx�N�g���ƃv���C���[�܂ł̕����x�N�g���̊p�x�Ƌ��������߂�
	Vector3 enemy_to_player = *player->GetPosition() - *pEnemy->GetPosition();
	float length = enemy_to_player.Length();

	//--------------------------------------------------------------------------
	// �ߋ����\�͈͓��Ƀv���C���[�����邩�H
	if (pEnemy->InNearArea()) {	// ���čU��

		// �v���C���[���O���ɂ��邩�H
		if (pEnemy->InNearFrontArea()) {
			pEnemy->ChangeState(new CStateEnemyAttack(pEnemy, ENEMY_STATE_ATTACK_HOOK));
			return;
		}

		pEnemy->ChangeState(new CStateEnemyAttack(pEnemy, ENEMY_STATE_ATTACK_JUMP));
		return;
	}


	//--------------------------------------------------------------------------
	// ���ʃx�N�g���ƃv���C���[�܂ł̕����x�N�g���̊p�x�̍��������߂�
	float front_to_player_difference = fabs(atan2f(enemy_to_player.x, enemy_to_player.z) - atan2f(pEnemy->GetFront().x, pEnemy->GetFront().z));


	//--------------------------------------------------------------------------
	// �p�x������45�x��菬������Α��s�X�e�[�g�ɑJ��
	if (front_to_player_difference <= 45.0f * DEGREE_TO_RADIAN) {
		pEnemy->ChangeState(new CStateEnemyMove(pEnemy, STATE_RUN));
		return;
	}
	else {// �p�x������45�x���傫����Ή�]�X�e�[�g�ɑJ��
		pEnemy->ChangeState(new CStateEnemyMove(pEnemy, STATE_ROTATE));
		return;
	}
}

