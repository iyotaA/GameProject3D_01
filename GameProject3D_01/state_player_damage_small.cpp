#include "state_player_damage_small.h"
#include "game_objects_all.h"
#include "state_player_idle.h"
#include "player.h"

CStatePlayerDamageSmall::CStatePlayerDamageSmall(CPlayer* pPlayer, int damage)
	: m_FrameCounter(0)
{
	pPlayer->SetAnimation(PLAYER_STATE_DAMAGE_SMALL, 1.0f);
	pPlayer->SetAnimationSpeed(1.0f);

	// ダメージ処理
	pPlayer->Life() = (pPlayer->Life() > damage) ? pPlayer->Life() - damage : 0;

	// 効果音再生
	CSound::Play(SOUND_LABEL_SE_DON);
}

CStatePlayerDamageSmall::~CStatePlayerDamageSmall()
{

}

void CStatePlayerDamageSmall::UpdateDamageState(CStatePlayerDamage* pStateDamage, CPlayer* pPlayer)
{
	// カウンター更新
	m_FrameCounter++;

	if (m_FrameCounter < 30) return;

	// ダメージステート終了
	pPlayer->Damaged() = false;
	pStateDamage->ChangeState(new CStatePlayerDamageNone(pPlayer));
}
