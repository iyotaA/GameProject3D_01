#include "game_objects_all.h"
#include "state_player_dodge.h"
#include "state_player_move.h"
#include "state_player_attack.h"
#include "state_player_attack_jump.h"
#include "state_player_attack_combo.h"
#include "state_player_attack_top.h"
#include "state_player_damage_large.h"
#include "modelAnimation.h"
#include "player.h"

CStatePlayerAttackJump::CStatePlayerAttackJump(CPlayer* pPlayer, bool isMove)
	: m_FrameCounter(0)
	, m_Velocity(Vector3())
	, m_DirFront(pPlayer->GetFront())
	, m_Move(isMove)
{
	pPlayer->SetAnimation(PLAYER_STATE_ATTACK_JUMP, 1.0f);
	pPlayer->SetAnimationSpeed(1.75f);

	// �U���͐ݒ�
	pPlayer->AttackValue() = 45;
}

CStatePlayerAttackJump::~CStatePlayerAttackJump()
{

}

void CStatePlayerAttackJump::UpdateAttackState(CStatePlayerAttack* pAttackState, CPlayer* pPlayer)
{
	// �ړ�����
	Move(pPlayer);
	m_FrameCounter++;

	// �A�j���[�V�����̃u�����h���I���܂ňȉ��̏��������s�����Ȃ�
	if (pPlayer->AnimationBlending()) return;

	// �U���\��Ԃɂ���(50 �` 70)
	if (m_FrameCounter == 50) {
		pPlayer->Attacked() = true;
	}

	// ���ʉ��Đ�
	if (m_FrameCounter == 60) {	CSound::Play(SOUND_LABEL_SE_SWING); }

	// ���͉\�܂ł̃t���[��
	if (m_FrameCounter < 70)return;

	///////////////////////////////////////
	// �L�[���͂Ŏ��̍U���X�e�[�g�ɑJ��
	///////////////////////////////////////
	if (CInput::GetGamepadTrigger(XINPUT_GAMEPAD_Y) || CInput::GetKeyTrigger('I')) {

		// *** �R���{�U�� ***
		pPlayer->Attacked() = false;
		pAttackState->ChangeState(new CStatePlayerAttackCombo(pPlayer));
		return;
	}
	if (CInput::GetGamepadTrigger(XINPUT_GAMEPAD_A) || CInput::GetKeyTrigger('K')) {

		// *** ��� ***
		pPlayer->Attacked() = false;
		pPlayer->ChangeState(new CStatePlayerDodge(pPlayer));
		return;
	}


	if (m_FrameCounter <= 110) return;

	pPlayer->Attacked() = false;
	pAttackState->ChangeState(new CStatePlayerAttackNone(pPlayer));
}

void CStatePlayerAttackJump::Move(CPlayer* pPlayer)
{
	// �����͗L�����H
	if (!m_Move)return;

	// 68F�o���Ă��Ȃ����H
	if (m_FrameCounter >= 68)return;

	if (m_FrameCounter <= 10) {
		m_Velocity += m_DirFront * pPlayer->DefaultSpeed();
		m_Velocity *= 0.95f;
		pPlayer->AddVelocity(m_Velocity);
		return;
	}

	m_Velocity *= 0.97f;
	pPlayer->AddVelocity(m_Velocity);
}
