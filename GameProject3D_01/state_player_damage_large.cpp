#include "game_objects_all.h"
#include "state_player_idle.h"
#include "state_player_damage_large.h"
#include "modelAnimation.h"
#include "player.h"

CStatePlayerDamageLarge::CStatePlayerDamageLarge(CPlayer* pPlayer)
	: m_FrameCounter(0)
	, m_Velocity(Vector3())
	, m_DirFront(-pPlayer->GetFront())
{
	pPlayer->SetAnimation(PLAYER_STATE_DAMAGE_LARGE, 1.0f);
	pPlayer->SetAnimationSpeed(1.0f);
}

CStatePlayerDamageLarge::~CStatePlayerDamageLarge()
{

}

void CStatePlayerDamageLarge::UpdateDamageState(CStatePlayerDamage* pStateDamage, CPlayer* pPlayer)
{
	// 移動処理
	Move(pPlayer);

	// カウンター更新
	m_FrameCounter++;

	if (pPlayer->AnimationBlending()) return;
	if (m_FrameCounter <= pPlayer->GetCurrentAnimFrameNum() - 20) return;

	// ダメージステート終了
	pPlayer->Damaged() = false;
	pStateDamage->ChangeState(new CStatePlayerDamageNone(pPlayer));
}

void CStatePlayerDamageLarge::Move(CPlayer* pPlayer)
{
	if (m_FrameCounter >= 68)return;

	if (m_FrameCounter <= 25) {
		m_Velocity += m_DirFront * pPlayer->DefaultSpeed();
		m_Velocity *= 0.88f;
		pPlayer->AddVelocity(m_Velocity);
		return;
	}

	m_Velocity *= 0.94f;
	pPlayer->AddVelocity(m_Velocity);
}
