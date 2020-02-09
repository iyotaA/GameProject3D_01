#include "game_objects_all.h"
#include "state_player_dodge.h"
#include "state_player_move.h"
#include "state_player_attack.h"
#include "state_player_attack_horizontal.h"
#include "state_player_attack_virtical.h"
#include "state_player_attack_low.h"
#include "modelAnimation.h"
#include "player.h"

CStatePlayerAttackHorizontal::CStatePlayerAttackHorizontal(CPlayer* pPlayer)
	: m_FrameCounter(0)
	, m_Velocity(Vector3())
	, m_DirFront(pPlayer->GetFront())
{
	pPlayer->SetAnimation(PLAYER_STATE_ATTACK_HORIZONTAL, 1.0f);
	pPlayer->SetAnimationSpeed(1.25f);
}

CStatePlayerAttackHorizontal::~CStatePlayerAttackHorizontal()
{

}

void CStatePlayerAttackHorizontal::UpdateAttackState(CStatePlayerAttack* pAttackState, CPlayer* pPlayer)
{
	// �ړ�����
	Move(pPlayer);
	m_FrameCounter++;

	// �U���\��Ԃɂ���
	if (m_FrameCounter == 40) {

		CSound::Play(SOUND_LABEL_SE_SWING); // ���ʉ��Đ�
		pPlayer->Attacked() = true;
	}

	// ���͉\�܂ł̃t���[��
	if (m_FrameCounter <= 70)return;

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
		pAttackState->ChangeState(new CStatePlayerAttackVirtical(pPlayer));
		return;
	}

	// �A�j���[�V�������I��莟��X�e�[�g�J��
	if (m_FrameCounter <= pPlayer->GetCurrentAnimFrameNum() / 1.25f) return;

	pPlayer->Attacked() = false;
	pAttackState->ChangeState(new CStatePlayerAttackNone(pPlayer));
}

void CStatePlayerAttackHorizontal::Move(CPlayer* pPlayer)
{

}
