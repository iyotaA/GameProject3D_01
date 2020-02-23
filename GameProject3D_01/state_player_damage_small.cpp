#include "state_player_damage_small.h"
#include "game_objects_all.h"
#include "state_player_idle.h"
#include "player.h"

CStatePlayerDamageSmall::CStatePlayerDamageSmall(CPlayer* pPlayer, int damage)
	: m_FrameCounter(0)
{
	pPlayer->SetAnimation(PLAYER_STATE_DAMAGE_SMALL, 1.0f);
	pPlayer->SetAnimationSpeed(1.0f);

	// �_���[�W����
	pPlayer->Life() = (pPlayer->Life() > damage) ? pPlayer->Life() - damage : 0;

	// ���ʉ��Đ�
	CSound::Play(SOUND_LABEL_SE_DON);
}

CStatePlayerDamageSmall::~CStatePlayerDamageSmall()
{

}

void CStatePlayerDamageSmall::UpdateDamageState(CStatePlayerDamage* pStateDamage, CPlayer* pPlayer)
{
	// �J�E���^�[�X�V
	m_FrameCounter++;

	if (m_FrameCounter < 30) return;

	// �_���[�W�X�e�[�g�I��
	pPlayer->Damaged() = false;
	pStateDamage->ChangeState(new CStatePlayerDamageNone(pPlayer));
}
