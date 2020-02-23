#include "game_objects_all.h"
#include "state_player_dodge.h"
#include "state_player_move.h"
#include "state_player_attack.h"
#include "state_player_attack_low.h"
#include "player.h"

CStatePlayerAttackLow::CStatePlayerAttackLow(CPlayer* pPlayer)
	: m_FrameCounter(0)
	, m_Velocity(Vector3())
	, m_DirFront(pPlayer->GetFront())
{
	pPlayer->SetAnimation(PLAYER_STATE_ATTACK_LOW, 1.0f);
	pPlayer->SetAnimationSpeed(1.5f);

	// �U���͐ݒ�
	pPlayer->AttackValue() = 32;
}

CStatePlayerAttackLow::~CStatePlayerAttackLow()
{

}

void CStatePlayerAttackLow::UpdateAttackState(CStatePlayerAttack* pAttackState, CPlayer* pPlayer)
{
	// �ړ�����
	Move(pPlayer);
	m_FrameCounter++;

	// �U���\��Ԃɂ���
	if (m_FrameCounter == 40) {

		CSound::Play(SOUND_LABEL_SE_SWING); // ���ʉ��Đ�
		pPlayer->Attacked() = true;
	}

	// ���͉\�܂ł̃t���[��(F)
	if (m_FrameCounter < 60)return;

	if (CInput::GetGamepadTrigger(XINPUT_GAMEPAD_A) || CInput::GetKeyTrigger('K')) {

		// *** ��� ***
		pPlayer->Attacked() = false;
		pPlayer->ChangeState(new CStatePlayerDodge(pPlayer));
		return;
	}


	// �A�j���[�V�����̃u�����h���I���܂ňȉ��̏��������s�����Ȃ�
	if (m_FrameCounter <= pPlayer->GetCurrentAnimFrameNum() / 1.5f - 10) return;

	pPlayer->Attacked() = false;
	pAttackState->ChangeState(new CStatePlayerAttackNone(pPlayer));
}

void CStatePlayerAttackLow::Move(CPlayer* pPlayer)
{

}
