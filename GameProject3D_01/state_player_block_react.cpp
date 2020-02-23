#include "game_objects_all.h"
#include "state_player_idle.h"
#include "state_player_damage_large.h"
#include "state_player_damage_small.h"
#include "player.h"
#include "state_player_block_idle.h"
#include "state_player_block_react.h"

CStatePlayerBlockReact::CStatePlayerBlockReact(CStatePlayerBlock* pStateBlock, CPlayer* pPlayer)
	: m_FrameCounter(0)
	, m_Velocity(Vector3())
	, m_ForceDirection(-pPlayer->GetFront())
{
	// 効果音再生
	CSound::Play(SOUND_LABEL_SE_GARD);

	pPlayer->SetAnimation(PLAYER_STATE_BLOCK_REACT, 1.0f);
	pPlayer->SetAnimationSpeed(2.0f);
	pPlayer->Life() = (pPlayer->Life() > 2) ? pPlayer->Life() - 2 : 0;

	// ガード有効時間計測開始
	pStateBlock->StartGardEnable();
}

CStatePlayerBlockReact::~CStatePlayerBlockReact()
{
}

void CStatePlayerBlockReact::UpdateBlockState(CStatePlayerBlock* pStateBlock, CPlayer* pPlayer)
{
	// 移動処理
	Move(pPlayer);

	// カウンター更新
	m_FrameCounter++;

	// アニメーション再生終了で防御待機ステートに遷移
	if (m_FrameCounter >= 20) {
		pStateBlock->ChangeState(new CStatePlayerBlockIdle(pPlayer));
	}
}

void CStatePlayerBlockReact::Move(CPlayer* pPlayer)
{
}
