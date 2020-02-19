#include "game_objects_all.h"
#include "state_player_idle.h"
#include "state_player_damage_large.h"
#include "state_player_damage_small.h"
#include "modelAnimation.h"
#include "player.h"
#include "enemy.h"
#include "state_player_damage.h"
#include "state_player_died.h"

CStatePlayerDamage::CStatePlayerDamage(CPlayer* pPlayer, int damage)
{
	switch (damage)
	{
	case 25:
		m_pStateDamage = new CStatePlayerDamageSmall(pPlayer, 25);
		break;
	case 35:
		m_pStateDamage = new CStatePlayerDamageLarge(pPlayer, 35);
		break;
	case 40:
		m_pStateDamage = new CStatePlayerDamageLarge(pPlayer, 40);
		break;
	default:
		m_pStateDamage = new CStatePlayerDamageNone(pPlayer);
	}
}

CStatePlayerDamage::~CStatePlayerDamage()
{
	delete m_pStateDamage;
}

void CStatePlayerDamage::Update(CPlayer* pPlayer)
{
	// 死亡ステートに遷移
	if (pPlayer->Life() <= 0.0f) {
		pPlayer->ChangeState(new CStatePlayerDied(pPlayer));
		return;
	}

	// ダメージステートが無ければ待機ステートに
	if (typeid(*m_pStateDamage) == typeid(CStatePlayerDamageNone)) {

		pPlayer->ChangeState(new CStatePlayerIdle(pPlayer));
		return;
	}

	// 更新
	m_pStateDamage->UpdateDamageState(this, pPlayer);
}

void CStatePlayerDamage::ChangeState(CStatePlayerDamage* pStateDamage)
{
	delete m_pStateDamage;
	m_pStateDamage = pStateDamage;
}

