#include "state_player_damage_small.h"
#include "game_objects_all.h"
#include "state_player_idle.h"
#include "modelAnimation.h"
#include "player.h"

CStatePlayerDamageSmall::CStatePlayerDamageSmall(CPlayer* pPlayer)
	: m_FrameCounter(0)
{
	pPlayer->SetAnimation(PLAYER_STATE_DAMAGE_SMALL, 1.0f);
	pPlayer->SetAnimationSpeed(1.0f);
}

CStatePlayerDamageSmall::~CStatePlayerDamageSmall()
{

}

void CStatePlayerDamageSmall::UpdateDamageState(CStatePlayerDamage* pStateDamage, CPlayer* pPlayer)
{
	// �J�E���^�[�X�V
	m_FrameCounter++;

	if (pPlayer->AnimationBlending()) return;
	if (m_FrameCounter <= pPlayer->GetCurrentAnimFrameNum()) return;

	// �_���[�W�X�e�[�g�I��
	pStateDamage->ChangeState(new CStatePlayerDamageNone(pPlayer));
}
