#include "state_player.h"
#include "state_player_move_run.h"
#include "state_player_idle.h"
#include "state_player_dodge.h"
#include "game_objects_all.h"
#include "modelAnimation.h"
#include "player.h"

CStatePlayerRun::CStatePlayerRun(CPlayer* pPlayer)
	: m_MoveSpeed(1.0f)
	, m_AnimationSpeed(1.0f)
{
	pPlayer->SetAnimation(2, 1.0f);
	pPlayer->SetAnimationSpeed(1.0f);
}

CStatePlayerRun::~CStatePlayerRun()
{

}

void CStatePlayerRun::Update(CPlayer* pPlayer)
{
	//******************************
	// ƒL[“ü—Í
	//******************************
	if (CInput::GetKeyTrigger(VK_SPACE) || CInput::GetGamepadTrigger(VK_PAD_A)) {
		pPlayer->ChangeState(new CStatePlayerDodge(pPlayer));
		return;
	}
	if (CInput::GetGamepadPress(XINPUT_GAMEPAD_RIGHT_SHOULDER))
	{
		m_MoveSpeed += 0.1f;
		m_AnimationSpeed += 0.1f;
	}

	m_MoveSpeed *= 0.95f;
	if (m_MoveSpeed <= 1.0f) { m_MoveSpeed = 1.0f; }

	m_AnimationSpeed *= 0.93f;
	if (m_AnimationSpeed <= 1.0f) { m_AnimationSpeed = 1.0f; }

	pPlayer->SetAnimationSpeed(m_AnimationSpeed);
	pPlayer->SetMoveSpeed(m_MoveSpeed);
}
