#include "game_objects_all.h"
#include "state_player_dodge.h"
#include "state_player_move.h"
#include "state_player_attack.h"
#include "state_player_attack_virtical.h"
#include "state_player_attack_top.h"
#include "state_player_attack_low.h"
#include "state_player_attack_horizontal.h"
#include "modelAnimation.h"
#include "player.h"

CStatePlayerAttackVirtical::CStatePlayerAttackVirtical(CPlayer* pPlayer)
	: m_FrameCounter(0)
	, m_Velocity(Vector3())
	, m_DirFront(pPlayer->GetFront())
{
	pPlayer->SetAnimation(PLAYER_STATE_ATTACK_VIRTICAL, 1.0f);
	pPlayer->SetAnimationSpeed(1.5f);
}

CStatePlayerAttackVirtical::~CStatePlayerAttackVirtical()
{

}

void CStatePlayerAttackVirtical::UpdateAttackState(CStatePlayerAttack* pAttackState, CPlayer* pPlayer)
{
	// �ړ�����
	Move(pPlayer);
	m_FrameCounter++;

	// �A�j���[�V�����̃u�����h���I���܂ňȉ��̏��������s�����Ȃ�
	if (pPlayer->AnimationBlending()) return;

	// �U���\��Ԃɂ���
	if (m_FrameCounter == 20) {

		CSound::Play(SOUND_LABEL_SE_SWING); // ���ʉ��Đ�
		pPlayer->Attacked() = true;
	}

	// ���͉\�܂ł̃t���[��
	if (m_FrameCounter <= 45)return;

	///////////////////////////////////////
	// �L�[���͂Ŏ��̍U���X�e�[�g�ɑJ��
	///////////////////////////////////////
	if (CInput::GetGamepadTrigger(XINPUT_GAMEPAD_Y) || CInput::GetKeyTrigger('I')) {

		pPlayer->Attacked() = false;
		pAttackState->ChangeState(new CStatePlayerAttackLow(pPlayer));
		return;
	}
	if (CInput::GetGamepadTrigger(XINPUT_GAMEPAD_B) || CInput::GetKeyTrigger('O')) {

		pPlayer->Attacked() = false;
		pAttackState->ChangeState(new CStatePlayerAttackHorizontal(pPlayer));
		return;
	}

	if (m_FrameCounter <= pPlayer->GetCurrentAnimFrameNum() / 1.5f) return;

	pPlayer->Attacked() = false;
	pAttackState->ChangeState(new CStatePlayerAttackNone(pPlayer));
}

void CStatePlayerAttackVirtical::Move(CPlayer* pPlayer)
{

}
