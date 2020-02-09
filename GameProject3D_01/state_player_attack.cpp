#include "game_objects_all.h"
#include "state_player_dodge.h"
#include "state_player_move.h"
#include "state_player_idle.h"
#include "state_player_attack.h"
#include "state_player_attack_jump.h"
#include "state_player_attack_virtical.h"
#include "state_player_damage.h"
#include "modelAnimation.h"
#include "player.h"

CStatePlayerAttack::CStatePlayerAttack(CPlayer* pPlayer, PLAYER_STATE state_index)
	: m_Lerp_t(0.0f)
	, m_LerpStart(CCameraManager::GetCamera()->GetLengthToAt())
	, m_LerpEnd(8.0f)
	, m_Counter(0.0f)
{
	if (state_index == PLAYER_STATE_ATTACK_JUMP) {
		m_pStateAttack = new CStatePlayerAttackJump(pPlayer, true);
	}
	else if (state_index == PLAYER_STATE_ATTACK_VIRTICAL) {
		m_pStateAttack = new CStatePlayerAttackVirtical(pPlayer);
	}
}

CStatePlayerAttack::~CStatePlayerAttack()
{
	delete m_pStateAttack;
}

void CStatePlayerAttack::Update(CPlayer* pPlayer)
{
	// ダメージステートに遷移
	if (pPlayer->Damaged()) {
		pPlayer->ChangeState(new CStatePlayerDamage(pPlayer));
		return;
	}

	// 攻撃ステートがセットされていなければ待機ステートに遷移
	if (typeid(*m_pStateAttack) == typeid(CStatePlayerAttackNone)) {

		pPlayer->Attacked() = false;	// 攻撃状態解除
		pPlayer->ChangeState(new CStatePlayerIdle(pPlayer));
		return;
	}

	// 更新
	m_pStateAttack->UpdateAttackState(this, pPlayer);
}

void CStatePlayerAttack::ChangeState(CStatePlayerAttack* pAttackState)
{
	delete m_pStateAttack;
	m_pStateAttack = pAttackState;
}
