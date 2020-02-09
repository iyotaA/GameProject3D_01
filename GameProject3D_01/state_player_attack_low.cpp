#include "game_objects_all.h"
#include "state_player_dodge.h"
#include "state_player_move.h"
#include "state_player_attack.h"
#include "state_player_attack_low.h"
#include "modelAnimation.h"
#include "player.h"

CStatePlayerAttackLow::CStatePlayerAttackLow(CPlayer* pPlayer)
	: m_FrameCounter(0)
	, m_Velocity(Vector3())
	, m_DirFront(pPlayer->GetFront())
{
	pPlayer->SetAnimation(PLAYER_STATE_ATTACK_LOW, 1.0f);
	pPlayer->SetAnimationSpeed(1.2f);
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

	// �A�j���[�V�����̃u�����h���I���܂ňȉ��̏��������s�����Ȃ�
	if (m_FrameCounter <= pPlayer->GetCurrentAnimFrameNum() / 1.2f - 10) return;

	pPlayer->Attacked() = false;
	pAttackState->ChangeState(new CStatePlayerAttackNone(pPlayer));
}

void CStatePlayerAttackLow::Move(CPlayer* pPlayer)
{

}
