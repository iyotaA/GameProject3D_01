#include "game_objects_all.h"
#include "state_player_idle.h"
#include "state_player_damage_large.h"
#include "state_player_damage_small.h"
#include "player.h"
#include "state_player_block.h"
#include "state_player_block_idle.h"
#include "state_player_died.h"

CStatePlayerBlock::CStatePlayerBlock(CPlayer* pPlayer)
	: m_pStateBlock(new CStatePlayerBlockIdle(pPlayer))
	, m_FrameBeteweenGard(0)
	, m_FrameCountStart(false)
{

}

CStatePlayerBlock::~CStatePlayerBlock()
{
	delete m_pStateBlock;
}

void CStatePlayerBlock::Update(CPlayer* pPlayer)
{
	// ���S�X�e�[�g�ɑJ��
	if (pPlayer->Life() <= 0.0f) {
		pPlayer->ChangeState(new CStatePlayerDied(pPlayer));
		return;
	}

	if (m_FrameCountStart) {
		m_FrameBeteweenGard++;
	}

	// ���g���K�[�A�����[�X�őҋ@�X�e�[�g�ɑJ��
	if (CInput::GetGamepadRelease(LEFT_TRRIGER) || CInput::GetKeyRelease(VK_LSHIFT))
	{
		pPlayer->Damaged() = false;
		pPlayer->ChangeState(new CStatePlayerIdle(pPlayer));
		return;
	}

	// �K�[�h���L���ȃt���[����
	if (m_FrameBeteweenGard == 100) {
		m_FrameBeteweenGard = 0;
		pPlayer->Damaged() = false;
		m_FrameCountStart = false;
	}

	// �X�V
	m_pStateBlock->UpdateBlockState(this, pPlayer);
}

void CStatePlayerBlock::ChangeState(CStatePlayerBlock* pStateDamage)
{
	delete m_pStateBlock;
	m_pStateBlock = pStateDamage;
}

