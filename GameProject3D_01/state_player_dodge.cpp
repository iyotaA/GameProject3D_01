#include "game_objects_all.h"
#include "state_player_dodge.h"
#include "state_player_move.h"
#include "state_player_idle.h"
#include "modelAnimation.h"
#include "player.h"

CStatePlayerDodge::CStatePlayerDodge(CPlayer* pPlayer)
	: m_FrameCounter(0)
	, m_Velocity(Vector3())
	, m_DirFront(pPlayer->GetFront())
{
	pPlayer->SetAnimation(PLAYER_STATE_ROOL, 1.0f);
	pPlayer->SetAnimationSpeed(1.2f);
	pPlayer->SetMoveSpeed(1.0f);
}

CStatePlayerDodge::~CStatePlayerDodge()
{
}

void CStatePlayerDodge::Update(CPlayer* pPlayer)
{
	// ˆÚ“®ˆ—
	Move(pPlayer);

	if (pPlayer->AnimationBlending()) return;
	if (m_FrameCounter <= pPlayer->GetCurrentAnimFrameNum() - 40) return;

	// ˆÚ“®“ü—Í‚³‚ê‚½‚©H
	MoveEntry() ?
		pPlayer->ChangeState(new CStatePlayerMove(pPlayer)) // true
		: pPlayer->ChangeState(new CStatePlayerIdle(pPlayer)); // else
}

void CStatePlayerDodge::Move(CPlayer* pPlayer)
{
	if (m_FrameCounter++ <= 25) {
		m_Velocity += m_DirFront * pPlayer->DefaultSpeed();
		m_Velocity *= 0.9f;
		pPlayer->AddVelocity(m_Velocity);
		return;
	}

	m_Velocity *= 0.99f;
	pPlayer->AddVelocity(m_Velocity);
}
