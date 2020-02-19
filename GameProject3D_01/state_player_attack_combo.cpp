#include "game_objects_all.h"
#include "state_player_dodge.h"
#include "state_player_move.h"
#include "state_player_idle.h"
#include "state_player_attack.h"
#include "state_player_attack_combo.h"
#include "state_player_attack_horizontal.h"
#include "state_player_attack_jump.h"
#include "modelAnimation.h"
#include "player.h"

CStatePlayerAttackCombo::CStatePlayerAttackCombo(CPlayer* pPlayer)
	: m_FrameCounter(0)
	, m_Velocity(Vector3())
	, m_DirFront(pPlayer->GetFront())
{
	pPlayer->SetAnimation(PLAYER_STATE_ATTACK_COMBO, 1.0f);
	pPlayer->SetAnimationSpeed(2.0f);

	// �U���͐ݒ�
	pPlayer->AttackValue() = 40;
}

CStatePlayerAttackCombo::~CStatePlayerAttackCombo()
{

}

void CStatePlayerAttackCombo::UpdateAttackState(CStatePlayerAttack* pAttackState, CPlayer* pPlayer)
{
	// �ړ�����
	Move(pPlayer);
	m_FrameCounter++;

	// �A�j���[�V�����̃u�����h���I���܂ňȉ��̏��������s�����Ȃ�
	if (pPlayer->AnimationBlending()) return;

	// �U���\��Ԃɂ���(60 �` 70F)
	if (m_FrameCounter == 60) {

		CSound::Play(SOUND_LABEL_SE_SWING); // ���ʉ��Đ�
		pPlayer->Attacked() = true;
	}
	if (m_FrameCounter == 70) {
		pPlayer->Attacked() = false;
	}

	// �U���\��Ԃɂ���(80 �` 90F)
	if (m_FrameCounter == 80) {

		CSound::Play(SOUND_LABEL_SE_SWING); // ���ʉ��Đ�
		pPlayer->Attacked() = true;
	}

	// ���͉\�܂ł̃t���[��
	if (m_FrameCounter < 90)return;

	///////////////////////////////////////
	// �L�[���͂Ŏ��̍U���X�e�[�g�ɑJ��
	///////////////////////////////////////
	if (CInput::GetGamepadTrigger(XINPUT_GAMEPAD_Y) || CInput::GetKeyTrigger('I')) {

		// *** ���X�C���O�U�� ***
		pPlayer->Attacked() = false;
		pAttackState->ChangeState(new CStatePlayerAttackHorizontal(pPlayer));
		return;
	}
	if (CInput::GetGamepadTrigger(XINPUT_GAMEPAD_B) || CInput::GetKeyTrigger('O')) {

		// *** �W�����v�U�� ***
		pPlayer->Attacked() = false;
		pAttackState->ChangeState(new CStatePlayerAttackJump(pPlayer, true));
		return;
	}
	if (CInput::GetGamepadTrigger(XINPUT_GAMEPAD_A) || CInput::GetKeyTrigger('K')) {

		// *** ��� ***
		pPlayer->Attacked() = false;
		pPlayer->ChangeState(new CStatePlayerDodge(pPlayer));
		return;
	}


	// �������͂�������Αҋ@��ԂɑJ��
	if (m_FrameCounter <= pPlayer->GetCurrentAnimFrameNum() / 2.0) return;

	pPlayer->Attacked() = false;
	pAttackState->ChangeState(new CStatePlayerAttackNone(pPlayer));
}

void CStatePlayerAttackCombo::Move(CPlayer* pPlayer)
{

}
