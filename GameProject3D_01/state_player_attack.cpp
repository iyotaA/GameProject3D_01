#include "game_objects_all.h"
#include "state_player_dodge.h"
#include "state_player_move.h"
#include "state_player_idle.h"
#include "state_player_attack.h"
#include "state_player_attack_jump.h"
#include "modelAnimation.h"
#include "player.h"

CStatePlayerAttack::CStatePlayerAttack(CPlayer* pPlayer)
	: m_pStateAttack(new CStatePlayerAttackJump(pPlayer))
	, m_Lerp_t(0.0f)
	, m_LerpStart(CCameraManager::GetCamera()->GetLengthToAt())
	, m_LerpEnd(8.0f)
	, m_Counter(0.0f)
{

}

CStatePlayerAttack::~CStatePlayerAttack()
{
	delete m_pStateAttack;
}

void CStatePlayerAttack::Update(CPlayer* pPlayer)
{
	// XV
	if (typeid(*m_pStateAttack) == typeid(CStatePlayerAttackNone)) {

		pPlayer->ChangeState(new CStatePlayerIdle(pPlayer));
		return;
	}

	m_pStateAttack->UpdateAttackState(this, pPlayer);
}

void CStatePlayerAttack::ChangeState(CStatePlayerAttack* pAttackState)
{
	delete m_pStateAttack;
	m_pStateAttack = pAttackState;
}
