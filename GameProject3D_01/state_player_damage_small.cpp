#include "state_player_damage_small.h"
#include "game_objects_all.h"
#include "state_player_idle.h"
#include "modelAnimation.h"
#include "player.h"
#include "state_player_damage.h"

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
	if (pPlayer->AnimationBlending()) return;
	if (m_FrameCounter++ <= pPlayer->GetCurrentAnimFrameNum() - 40) return;

	// ダメージステート終了
	pStateDamage->ChangeState(new CStatePlayerDamageNone(pPlayer));
}
