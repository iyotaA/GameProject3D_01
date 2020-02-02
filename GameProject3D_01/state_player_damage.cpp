#include "game_objects_all.h"
#include "state_player_idle.h"
#include "state_player_damage_large.h"
#include "state_player_damage_small.h"
#include "modelAnimation.h"
#include "player.h"
#include "state_player_damage.h"

CStatePlayerDamage::CStatePlayerDamage(CPlayer* pPlayer)
	: m_pStateDamage(new CStatePlayerDamageLarge(pPlayer))
{

}

CStatePlayerDamage::~CStatePlayerDamage()
{
	delete m_pStateDamage;
}

void CStatePlayerDamage::Update(CPlayer* pPlayer)
{
	// �_���[�W�X�e�[�g��������Αҋ@�X�e�[�g��
	if (typeid(*m_pStateDamage) == typeid(CStatePlayerDamageNone)) {

		pPlayer->ChangeState(new CStatePlayerIdle(pPlayer));
		return;
	}

	// �X�V
	m_pStateDamage->UpdateDamageState(this, pPlayer);
}

void CStatePlayerDamage::ChangeState(CStatePlayerDamage* pStateDamage)
{
	delete m_pStateDamage;
	m_pStateDamage = pStateDamage;
}

